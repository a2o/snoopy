#!/bin/bash



### Shell configuration and script bootstrap
#
set -e
set -u
set -o pipefail
. `dirname $0`/../_bootstrap.sh



### Define the help method
#
_showHelp()
{
    cat <<EOF
Purpose:

    Extract Snoopy native package installation steps from doc/install-from-repo.md

Supported CLI arguments:

    -c          Same as -c, but get the version from ./dev-tools/libexec/get-release-version.sh
    -C VERSION  Check installed Snoopy version (only applicable in combination with -e flag)
    -d DIST     Distribution to extract steps for [default: use host system distribution]
    -e          Execute extracted steps too
    -p          Same as -p, but get the package version from ./dev-tools/build-native-package -v
    -P VERSION  Check installed Snoopy _package_ version (only applicable in combination with -e flag)
    -P
    -r          Switch repository URL to "raw":
                - From: https://a2o.github.io/snoopy-packages/repo/...
                - To:   https://raw.githubusercontent.com/a2o/snoopy-packages/master/repo/...
                The purpose of this switch is workflow-based package release testing - GitHub Pages
                sometimes take a bit to get refreshed.
    -t          Switch from 'stable' to 'testing' repositories
    -v VERSION  Distribution version to extract steps for [default: use host system distribution version]

    -h/--help   Show this help.

Usage:

    Just show the steps for a distribution running this script:
        $0

    Extract and execute:
        $0 -e

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

MODE="extract-only"
CHECK_INSTALLED_SNOOPY_LIBRARY_VERSION=""
CHECK_INSTALLED_SNOOPY_PACKAGE_VERSION=""
OS_ID_OVERRIDE=""
OS_VERSION_OVERRIDE=""
REPO_URL_SWITCH_TO_RAW="false"
REPO_CHANNEL_SWITCH_TO_TESTING="false"


while getopts ":cC:d:epP:rtv:h" opt; do
    case "$opt" in
        c)
            CHECK_INSTALLED_SNOOPY_LIBRARY_VERSION=`./dev-tools/libexec/get-release-version.sh`
            ;;

        C)
            CHECK_INSTALLED_SNOOPY_LIBRARY_VERSION="$OPTARG"
            ;;

        d)
            OS_ID_OVERRIDE="$OPTARG"
            ;;

        e)
            MODE="extract-and-execute"
            ;;

        p)
            CHECK_INSTALLED_SNOOPY_PACKAGE_VERSION=`./dev-tools/build-native-package.sh -v`
            ;;

        P)
            CHECK_INSTALLED_SNOOPY_PACKAGE_VERSION="$OPTARG"
            ;;

        r)
            REPO_URL_SWITCH_TO_RAW="true"
            ;;

        t)
            REPO_CHANNEL_SWITCH_TO_TESTING="true"
            ;;

        v)
            OS_VERSION_OVERRIDE="$OPTARG"
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



### Detect operating system, or use the overrides
#
OS_ID=""
OS_VERSION=""
_detectOperatingSystem

if [ "$OS_ID_OVERRIDE" != "" ]; then
    _echo "Using OS id override: $OS_ID_OVERRIDE"
    OS_ID="$OS_ID_OVERRIDE"
fi

if [ "$OS_VERSION_OVERRIDE" != "" ]; then
    _echo "Using OS version override: $OS_VERSION_OVERRIDE"
    OS_VERSION="$OS_VERSION_OVERRIDE"
fi



### Generate search key
#
if [ "$OS_VERSION" == "" ]; then
    SEARCH_KEY="$OS_ID"
else
    SEARCH_KEY="$OS_ID-$OS_VERSION"
fi
_echo "Using OS search key: $SEARCH_KEY"



### Extract the install steps
#
SEARCH_STRING="[search-key:$SEARCH_KEY]:"
SEARCH_FILE="doc/install-from-repo.md"
if ! grep -F "$SEARCH_STRING" "$SEARCH_FILE" > /dev/null ; then
    _fatalError "Unable to find package installation steps for string '$SEARCH_STRING' in file $SEARCH_FILE."
fi

INSTALL_STEPS=`cat $SEARCH_FILE | grep -F "$SEARCH_STRING" -A20 | sed -e '1,/^\`\`\`shell$/d' | sed -e '/^\`\`\`$/,$d' | sed -e 's/^sudo //'`
if [ "$INSTALL_STEPS" == "" ]; then
    _fatalError "Unable to extract package installation steps for search key '$SEARCH_KEY' from file $SEARCH_FILE."
fi



### Adjust if requested
#
if [ "$REPO_URL_SWITCH_TO_RAW" == "true" ]; then
    _echo "NOTICE: Switching repository URLs from GitHub Pages to raw.githubusercontent..."
    INSTALL_STEPS=`echo "$INSTALL_STEPS" | sed -e 's#https://a2o.github.io/snoopy-packages/repo/#https://raw.githubusercontent.com/a2o/snoopy-packages/master/repo/#g'`
fi

if [ "$REPO_CHANNEL_SWITCH_TO_TESTING" == "true" ]; then
    _echo "NOTICE: Switching instructions from 'stable' to 'testing'..."
    INSTALL_STEPS=`echo "$INSTALL_STEPS" | sed -e 's/stable/testing/g'`
fi



### Show extracted steps
#
echo "Extracted package installation steps ('sudo' was removed, not needed for CI):"
echo "============================================================================="
echo "$INSTALL_STEPS"
echo "============================================================================="



### Run the install steps if requested
#
if [ "$MODE" == "extract-and-execute" ]; then
    _echo "Executing extracted install steps:"
    eval $INSTALL_STEPS
fi




### Run the Snoopy library version check
#
if [ "$CHECK_INSTALLED_SNOOPY_LIBRARY_VERSION" != "" ]; then
    INSTALLED_SNOOPY_LIBRARY_VERSION=`snoopyctl version | grep '^Snoopy library version:' | awk '{print $4}'`
    if [ "$CHECK_INSTALLED_SNOOPY_LIBRARY_VERSION" != "$INSTALLED_SNOOPY_LIBRARY_VERSION" ]; then
        _fatalError "Installed Snoopy library version is not the same as expected (expected=$CHECK_INSTALLED_SNOOPY_LIBRARY_VERSION, installed=$INSTALLED_SNOOPY_LIBRARY_VERSION)"
    fi
fi


### FUNCTION: Get installed package's version
#
# Usage:
#       INSTALLED_PACKAGE_VERSION=""
#       _getInstalledPackageVersion "your-package-name-here"
#
_getInstalledPackageVersion() {
    PACKAGE_NAME="$1"

    OS_ID=""
    _detectOperatingSystem

    INSTALLED_PACKAGE_VERSION=""
    case "$OS_ID" in
        arch)
            INSTALLED_PACKAGE_VERSION=`pacman -Q "$PACKAGE_NAME" | awk '{print $2}'`
            ;;

        debian|ubuntu)
            INSTALLED_PACKAGE_VERSION=`dpkg-query --showformat='${Version}' --show "$PACKAGE_NAME"`
            ;;

        rhel|fedora|centos|almalinux|\
        sles|opensuse-leap|opensuse-tumbleweed)
            INSTALLED_PACKAGE_VERSION=`rpm -q "$PACKAGE_NAME" --qf '%{VERSION}'`
            ;;

        *)
            _fatalError "Unsupported OS: '$OS_ID'. Install Snoopy from source instead."
            ;;
    esac
}

### Run the Snoopy package version check
#
if [ "$CHECK_INSTALLED_SNOOPY_PACKAGE_VERSION" != "" ]; then
    INSTALLED_PACKAGE_VERSION=""
    _getInstalledPackageVersion "snoopy"
    if [ "$CHECK_INSTALLED_SNOOPY_PACKAGE_VERSION" != "$INSTALLED_PACKAGE_VERSION" ]; then
        _fatalError "Installed Snoopy package version is not the same as expected (expected=$CHECK_INSTALLED_SNOOPY_PACKAGE_VERSION, installed=$INSTALLED_PACKAGE_VERSION)"
    fi
fi
