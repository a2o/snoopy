#!/bin/bash



### Shell configuration and script bootstrap
#
set -e
set -u
set -o pipefail
. `dirname $0`/_bootstrap.sh



### Define the help method
#
_showHelp()
{
    cat <<EOF
Purpose:

    Publish a distribution-native Snoopy package

Supported CLI arguments:

    -b          Run a build if expected build file is not yet present.
    -f          Force - overwrite existing package in target repository when it already exists.
    -r N        Specify a custom release number to use.
                Gets passed to build-native-package.sh script when -b flag is specified.
                [default=1]

    -h/--help   Show this help.

Usage:

    Publish a package:
        $0

    Publish a package, but build it first if the package file is not yet present on the system:
        $0 -b

    Publish a package with a specific release number:
        $0 -r 2

EOF
}

_showHelpAndExit()
{
    _showHelp
    exit
}



### Parse the CLI arguments
#
if [[ $@ =~ [-][-]help ]]; then
    _showHelpAndExit
fi

ARG_BUILD_IF_PKG_NOT_FOUND="false"
ARG_OVERWRITE_TARGET_PKG="false"
ARG_PKG_RELEASE_NUMBER="1"

while getopts ":bfr:" opt; do
    case "$opt" in
        b)
            ARG_BUILD_IF_PKG_NOT_FOUND="true"
            ;;

        f)
            ARG_OVERWRITE_TARGET_PKG="true"
            ;;

        r)
            ARG_PKG_RELEASE_NUMBER="$OPTARG"
            ;;

        h)
            _showHelpAndExit
            ;;

        ?)
            _fatalError "Unsupported argument: '-$OPTARG'. Run '$0 -h' to list supported arguments." $LINENO
            ;;

        *)
            _fatalError "Internal error (opt=$opt)" $LINENO
            ;;
    esac
done



### Detect the OS
#
OS_ID=""
OS_VERSION=""
OS_VERSION_CODENAME=""
_detectOperatingSystem
if [ "$OS_VERSION_CODENAME" == "" ]; then
    unset OS_VERSION_CODENAME
fi



### Get Snoopy code versioning information
#
SNOOPY_RELEASE_TAG=`./dev-tools/libexec/get-release-tag.sh`
PACKAGING_CHANNEL=""
_getPackagingChannelFromTag "$SNOOPY_RELEASE_TAG"
_echo "Packaging channel to release into: $PACKAGING_CHANNEL"



### Get the package file information
#
PKG_FILE_NAME=`./dev-tools/build-native-package.sh -n -r $ARG_PKG_RELEASE_NUMBER`
PKG_FILE_PATH=`./dev-tools/build-native-package.sh -p -r $ARG_PKG_RELEASE_NUMBER`
_echo "Target package file name: $PKG_FILE_NAME"
_echo "Target package file path: $PKG_FILE_PATH"



### Build if necessary & requested
#
if [ ! -e "$PKG_FILE_PATH" ]; then
    if [ "$ARG_BUILD_IF_PKG_NOT_FOUND" == "true" ]; then
        _echo "Native package file does not exist, building it first:"
        ./dev-tools/build-native-package.sh -r "$ARG_PKG_RELEASE_NUMBER"
        _echo "Native package build complete."
    fi
fi

if [ ! -e "$PKG_FILE_PATH" ]; then
    _fatalError "Native package file not found: $PKG_FILE_PATH"
fi



### Clone the packages repository
#
rm -rf snoopy-packages
git clone git@github.com:a2o/snoopy-packages



### Publish the package
#
_copyPkgToTargetRepo() {
    PKG_FILE_PATH="$1"
    DEST_FILE_PATH="$2"
    OVERWRITING_ALLOWED="$3"

    if [ -e $DEST_FILE_PATH ]; then
        if [ "$OVERWRITING_ALLOWED" != "true" ]; then
            _fatalError "Target native package file already exists: $DEST_FILE_PATH"
        fi
    fi

    # Copy the package
    mkdir -p `dirname $DEST_FILE_PATH`
    rm -f $DEST_FILE_PATH
    cp $PKG_FILE_PATH $DEST_FILE_PATH
}

case "$OS_ID" in


    arch)
        ARCH_REPO_DIR="snoopy-packages/repo/$OS_ID/$PACKAGING_CHANNEL"
        ARCH_REPO_PACKAGES_DIR="$ARCH_REPO_DIR"
        DEST_FILE_PATH="$ARCH_REPO_PACKAGES_DIR/$PKG_FILE_NAME"

        # Copy the package into the repo
        _copyPkgToTargetRepo "$PKG_FILE_PATH" "$DEST_FILE_PATH" "$ARG_OVERWRITE_TARGET_PKG"

        # Sign the package
        gpg --yes --detach-sign --local-user snoopy-packages@a2o.github.com --output $DEST_FILE_PATH.sig $DEST_FILE_PATH

        # Update the pacman repository indexes
        (
            cd $ARCH_REPO_DIR

            # Regenerate the repo metadata
            # WARNING: File name (snoopy-stable.db.tar.zst) must match the
            #          name of the section in pacman.conf ("[snoopy-stable]").
            if [ "$ARG_OVERWRITE_TARGET_PKG" == "true" ]; then
                repo-add --sign --key snoopy-packages@a2o.github.com snoopy-$PACKAGING_CHANNEL.db.tar.zst *.pkg.tar.zst
            else
                # If not overwriting, only process new packages to optimize for speed
                repo-add --new --sign --key snoopy-packages@a2o.github.com snoopy-$PACKAGING_CHANNEL.db.tar.zst *.pkg.tar.zst
            fi

            # Remove stale files
            rm -f *.old
        )

        # Git commit && push
        (
            cd $ARCH_REPO_DIR

            git add .
            git commit -m "Release new $PACKAGING_CHANNEL package for $OS_ID: $PKG_FILE_NAME"
            git push
        )
        ;;


    debian|ubuntu)
        APT_REPO_DIR="snoopy-packages/repo/$OS_ID"
        DEST_DIR="snoopy-packages/repo/$OS_ID/pool/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL"
        DEST_FILE_PATH="$DEST_DIR/$PKG_FILE_NAME"

        # Copy the package into the repo
        _copyPkgToTargetRepo "$PKG_FILE_PATH" "$DEST_FILE_PATH" "$ARG_OVERWRITE_TARGET_PKG"

        # Update APT repo index files
        (
            cd $APT_REPO_DIR

            # Packages
            mkdir -p dists/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL/binary-amd64
            apt-ftparchive packages pool/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL > dists/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL/binary-amd64/Packages
            gzip  -f -k dists/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL/binary-amd64/Packages

            # Contents-amd64
            apt-ftparchive contents pool/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL > dists/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL/Contents-amd64
            gzip -f -k dists/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL/Contents-amd64

            # Release #1
            apt-ftparchive release dists/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL/binary-amd64 > dists/$OS_VERSION_CODENAME/$PACKAGING_CHANNEL/binary-amd64/Release

            # Release #2
            # NOTICE: Options must be specified without surrounding spaces around the '=' character.
            # NOTICE: All possible PACKAGING_CHANNEL values must appear in the 'Components' value.
            apt-ftparchive release \
                -o="APT::FTPArchive::Release::Origin=https://a2o.github.io/snoopy-packages/" \
                -o="APT::FTPArchive::Release::Suite=snoopy-upstream" \
                -o="APT::FTPArchive::Release::Codename=$OS_VERSION_CODENAME" \
                -o="APT::FTPArchive::Release::Components=stable testing" \
                -o="APT::FTPArchive::Release::Label=Snoopy Command Logger - Upstream APT repository" \
                -o="APT::FTPArchive::Release::Architectures=amd64" \
                  dists/$OS_VERSION_CODENAME \
                > dists/$OS_VERSION_CODENAME/Release

            # Sign
            gpg -a --yes --detach-sign --local-user snoopy-packages@a2o.github.com             --output dists/$OS_VERSION_CODENAME/Release.gpg dists/$OS_VERSION_CODENAME/Release
            gpg -a --yes --detach-sign --local-user snoopy-packages@a2o.github.com --clearsign --output dists/$OS_VERSION_CODENAME/InRelease   dists/$OS_VERSION_CODENAME/Release
        )

        # Git commit && push
        (
            cd $APT_REPO_DIR

            git add .
            git commit -m "Release new $PACKAGING_CHANNEL package for $OS_ID/$OS_VERSION_CODENAME: $PKG_FILE_NAME"
            git push
        )
        ;;


    rhel|fedora|centos|almalinux|\
    sles|opensuse-leap|opensuse-tumbleweed)
        YUM_REPO_DIR="snoopy-packages/repo/$OS_ID/$OS_VERSION/$PACKAGING_CHANNEL"
        YUM_REPO_PACKAGES_DIR="$YUM_REPO_DIR/Packages"
        DEST_FILE_PATH="$YUM_REPO_PACKAGES_DIR/$PKG_FILE_NAME"

        # Copy the package into the repo
        _copyPkgToTargetRepo "$PKG_FILE_PATH" "$DEST_FILE_PATH" "$ARG_OVERWRITE_TARGET_PKG"

        # Sign the package
        rpmsign --addsign --key-id=snoopy-packages@a2o.github.com $DEST_FILE_PATH

        # Update YUM repo index files
        (
            cd $YUM_REPO_DIR

            # Generate all metadata files
            if [ "$ARG_OVERWRITE_TARGET_PKG" == "true" ]; then
                createrepo .
            else
                createrepo --update .
            fi

            # Sign the repo
            gpg -a --yes --detach-sign --local-user snoopy-packages@a2o.github.com repodata/repomd.xml
        )

        # Git commit && push
        (
            cd $YUM_REPO_DIR

            git add .
            git commit -m "Release new $PACKAGING_CHANNEL package for $OS_ID/$OS_VERSION: $PKG_FILE_NAME"
            git push
        )
        ;;


    *)
        _fatalError "Unsupported OS: '$OS_ID'. Install Snoopy from source instead."
        ;;
esac



### Remove the packages repo clone
#
rm -rf snoopy-packages



### Report success
#
_echo ""
_echo "SUCCESS: Native package publishing complete."
_echo "SUCCESS: Published at: $DEST_FILE_PATH"
_echo ""
