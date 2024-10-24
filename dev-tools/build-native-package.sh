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

    Build a distribution-native Snoopy package

Supported CLI arguments:

    -n          Do not build, only show the final package's file name.
    -p          Do not build, only show the final package's file path.
    -r N        Specify a custom release number. [default=1]
    -v          Do not build, only show the final package's version.
    -V VERSION  Use this specific VERSION for generating package's version tag.
                (Intended to be used with -n or -v only.)

    -h/--help   Show this help.

Usage:

    Build a package:
        $0

    Build a package with a specific release number:
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

ARG_RUN_MODE="build"
ARG_PKG_RELEASE_NUMBER="1"
ARG_VERSION=""

while getopts ":npr:vV:h" opt; do
    case "$opt" in
        n)
            ARG_RUN_MODE="only-show-file-name"
            ;;

        p)
            ARG_RUN_MODE="only-show-file-path"
            ;;

        r)
            ARG_PKG_RELEASE_NUMBER="$OPTARG"
            ;;

        v)
            ARG_RUN_MODE="only-show-package-version"
            ;;

        V)
            ARG_VERSION="$OPTARG"
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



### Get the Snoopy release information
#
if [ "$ARG_VERSION" != "" ]; then
    SNOOPY_RELEASE_TAG="snoopy-$ARG_VERSION"
    SNOOPY_RELEASE_VERSION="$ARG_VERSION"
else
    SNOOPY_RELEASE_TAG=`./dev-tools/libexec/get-release-tag.sh`
    SNOOPY_RELEASE_VERSION=`echo "$SNOOPY_RELEASE_TAG" | sed -e 's/snoopy-//'`
fi
if [ "$SNOOPY_RELEASE_VERSION" == "" ]; then
    _fatalError "Unable to determine Snoopy version, got: '$SNOOPY_RELEASE_VERSION'"
fi



### Determine the package details
#
# The '-rc' -> '~rc' transformation causes the dpkg/dnf/yum to install the final
# release, once it is available, over the installed release candidates.
#
# On arch, the tilde character is not treated as above. However, i.e. version 1.0.0rc
# is considered lower than 1.0.0, so we remove the potential dash in front of rc.
#
case "$OS_ID" in

    arch)
        # For pacman, 1.0.0rc1 < 1.0.0
        PKG_SNOOPY_VERSION=`echo "$SNOOPY_RELEASE_VERSION" | sed -e 's/-rc/rc/' | sed -e 's/-/_/g'`
        PKG_RELEASE_NUMBER="$ARG_PKG_RELEASE_NUMBER"

        ARCHITECTURE=`uname -m`
        PKG_VERS_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}"
        PKG_FULL_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}-${ARCHITECTURE}"

        PKG_FILE_NAME="snoopy-${PKG_FULL_TAG}.pkg.tar.zst"
        PKG_FILE_PATH="${PKG_FILE_NAME}"
        ;;

    debian|ubuntu)
        # For dpkg/apt, 1.0.0~rc1 < 1.0.0
        PKG_SNOOPY_VERSION=`echo "$SNOOPY_RELEASE_VERSION" | sed -e 's/-\?rc/~rc/'`
        PKG_RELEASE_NUMBER="$ARG_PKG_RELEASE_NUMBER"
        PKG_RELEASE_DIST="${OS_VERSION_CODENAME}"
        PKG_RELEASE_TAG="${PKG_RELEASE_NUMBER}~${PKG_RELEASE_DIST}"

        ARCHITECTURE=`dpkg-architecture -q DEB_BUILD_ARCH`
        PKG_VERS_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}~${PKG_RELEASE_DIST}"
        PKG_FULL_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}~${PKG_RELEASE_DIST}_${ARCHITECTURE}"

        PKG_FILE_NAME="snoopy_${PKG_FULL_TAG}.deb"
        PKG_FILE_PATH="../${PKG_FILE_NAME}"
        ;;

    rhel|fedora|centos|almalinux)
        # For rpm/yum/dnf, 1.0.0~rc1 < 1.0.0
        PKG_SNOOPY_VERSION=`echo "$SNOOPY_RELEASE_VERSION" | sed -e 's/-\?rc/~rc/' | sed -e 's/-/_/g'`
        PKG_RELEASE_NUMBER="$ARG_PKG_RELEASE_NUMBER"
        PKG_RELEASE_DIST=`rpm --eval '%{?dist}' | sed -e 's/^[.]//'`
        if [ "$PKG_RELEASE_DIST" == "" ]; then
            _fatalError "Unable to determine target distribution tag"
        fi
        PKG_RELEASE_TAG="${PKG_RELEASE_NUMBER}.${PKG_RELEASE_DIST}"

        ARCHITECTURE=`rpm --eval '%{_arch}'`
        PKG_VERS_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}.${PKG_RELEASE_DIST}"
        PKG_FULL_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}.${PKG_RELEASE_DIST}.${ARCHITECTURE}"

        PKG_FILE_NAME="snoopy-${PKG_FULL_TAG}.rpm"
        PKG_FILE_PATH="packaging/rpmbuild/RPMS/$ARCHITECTURE/${PKG_FILE_NAME}"
        ;;

    sles|opensuse-leap|opensuse-tumbleweed)
        # For rpm/zypper, 1.0.0~rc1 < 1.0.0
        PKG_SNOOPY_VERSION=`echo "$SNOOPY_RELEASE_VERSION" | sed -e 's/-\?rc/~rc/' | sed -e 's/-/_/g'`
        PKG_RELEASE_NUMBER="$ARG_PKG_RELEASE_NUMBER"
        case "$OS_VERSION" in
            "15.4")
                PKG_RELEASE_DIST="bp154"
                ;;
            "15.5")
                PKG_RELEASE_DIST="bp155"
                ;;
            *)
                _fatalError "Unsupported SUSE version: $OS_VERSION"
                ;;
        esac
        PKG_RELEASE_TAG="${PKG_RELEASE_NUMBER}.${PKG_RELEASE_DIST}"

        ARCHITECTURE=`rpm --eval '%{_arch}'`
        PKG_VERS_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}.${PKG_RELEASE_DIST}"
        PKG_FULL_TAG="${PKG_SNOOPY_VERSION}-${PKG_RELEASE_NUMBER}.${PKG_RELEASE_DIST}.${ARCHITECTURE}"

        PKG_FILE_NAME="snoopy-${PKG_FULL_TAG}.rpm"
        PKG_FILE_PATH="packaging/rpmbuild/RPMS/$ARCHITECTURE/${PKG_FILE_NAME}"
        ;;

    *)
        _fatalError "Unsupported OS: '$OS_ID'. Install Snoopy from source instead."
        ;;
esac



### Special "show" run modes
#
if [ "$ARG_RUN_MODE" == "only-show-file-name" ]; then
    echo "$PKG_FILE_NAME"
    exit
fi
if [ "$ARG_RUN_MODE" == "only-show-file-path" ]; then
    echo "$PKG_FILE_PATH"
    exit
fi
if [ "$ARG_RUN_MODE" == "only-show-package-version" ]; then
    echo "$PKG_VERS_TAG"
    exit
fi



### Show info in the default "build" run mode
#
_echo "Snoopy code version:      $SNOOPY_RELEASE_VERSION"
_echo "Target package full tag:  $PKG_FULL_TAG"
_echo "Target package file name: $PKG_FILE_NAME"
_echo "Target package file path: $PKG_FILE_PATH"



### Verify version consistency for public releases
#
if _doesReleaseTagDenotePublicRelease "$SNOOPY_RELEASE_TAG" ; then
    _echo "This is a public rc/stable build, running version consistency checks..."
    ./dev-tools/libexec/verify-last-version-everywhere.sh "$SNOOPY_RELEASE_VERSION"
else
    _echo "This is NOT a public rc/stable build, thus skipping version consistency checks."
fi



### Does the target release package already exist?
#
if [ -e $PKG_FILE_PATH ]; then
    _fatalError "Release package file already exists: $PKG_FILE_PATH (Hint: 'rm -f $PKG_FILE_PATH' or 'make clean')"
fi



### Clean the environment if necessary
#
./dev-tools/clean-git-repository.sh



### Run the build environment bootstrap
#
./bootstrap.sh



### Build the package
#
case "$OS_ID" in


    arch)
        # Make gitclean removes this one, so let's (re?)create it
        rm -f PKGBUILD
        ln -s packaging/arch/PKGBUILD

        # Pull in some settings as environment variables
        source packaging/arch/env

        # Build the package
        makepkg --holdver

        # Cleanup
        git checkout packaging/arch/PKGBUILD
        rm -rf pkg/
        ;;


    debian|ubuntu)
        # Symlink the debian/ directory first
        rm -f debian
        ln -s packaging/deb debian

        # Generate "proper" packaging/deb/changelog content
        #
        # Why such a convoluted approach?
        #
        # Well, we can't do that with ./configure that translates
        # `changelog.in` template to `changelog`, as dpkg-buildpackage first
        # cleans the directory, making subsequent step fail due to a missing
        #  `changelog` file.
        cat packaging/deb/changelog.TEMPLATE \
        | sed -e "s/DEB_PACKAGE_VERSION/$PKG_VERS_TAG/" \
        > packaging/deb/changelog

        # Build the package
        dpkg-buildpackage --build=binary --no-sign

        # Remove the temporary changelog file
        rm -f packaging/deb/changelog

        # Cleanup
        rm -rf packaging/deb/.debhelper/
        rm -f  packaging/deb/autoreconf.after
        rm -f  packaging/deb/autoreconf.before
        rm -f  packaging/deb/debhelper-build-stamp
        rm -f  packaging/deb/files
        rm -f  packaging/deb/snoopy.debhelper.log
        rm -f  packaging/deb/snoopy.postrm.debhelper
        rm -f  packaging/deb/snoopy.substvars
        rm -rf packaging/deb/snoopy/
        rm -f  debian
        ;;


    rhel|fedora|centos|almalinux|\
    sles|opensuse-leap|opensuse-tumbleweed)
        rpmbuild \
            --define "snoopy_rpm_package_version ${PKG_SNOOPY_VERSION}" \
            --define "snoopy_rpm_release_tag ${PKG_RELEASE_TAG}" \
            --define "_topdir $PWD/packaging/rpmbuild" \
            --define '_configure ../../../configure' \
            -bb packaging/rpm/snoopy.spec
        ;;


    *)
        _fatalError "Unsupported OS: '$OS_ID'. Install Snoopy from source instead."
        ;;
esac



### Check if target file has indeed been built
#
if [ ! -e $PKG_FILE_PATH ]; then
    _fatalError "Package build complete, but expected package file not found: $PKG_FILE_PATH"
fi



### Report success
#
_echo ""
_echo "SUCCESS: Build complete."
_echo "SUCCESS: Package file path: $PKG_FILE_PATH"
_echo ""
