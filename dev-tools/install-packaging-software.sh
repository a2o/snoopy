#!/bin/bash



### How does this script operate?
#
# Script's expectations:
# - To be run on one of the supported OSes
#
# Script's arguments:
# - (none)
#
# Steps that this script performs:
# - Verifies the presence of required binaries
# - If all are present, exit
# - Detect the OS, bail out if unknown/unsupported
# - Use the OS-specific package installation method to intall all required packages
#



### Shell configuration and script bootstrap
#
set -e
set -u
set -o pipefail
BOOTSTRAP_GIT_REPO_REQUIRED="false"
. `dirname $0`/_bootstrap.sh



### Definitions
#
  PACKAGE_NAMES_ARCH="pacman"
PACKAGE_NAMES_DEBIAN="debhelper"
PACKAGE_NAMES_REDHAT="rpm-build"
  PACKAGE_NAMES_SUSE="rpm-build"



### Software check & install functions
#
# NOTICE: Keep this code in sync in the following files:
#   - dev-tools/install-dev-software.sh
#   - install/install-snoopy.sh
#
_detectOperatingSystem()
{
    # NOTICE: Keep this code in sync in the following files:
    #   - dev-tools/install-dev-software.sh
    #   - install/install-snoopy.sh
    #
    # Expects:
    #   - Global variable OS_ID set to ""
    #   - Global variable OS_VERSION set to ""
    #
    # Sets:
    #   - Global variable OS_ID
    #   - Global variable OS_VERSION
    #
    # Returns:
    #   - (nothing)
    OS_ID=""
    OS_VERSION=""

    . /etc/os-release
    OS_ID="$ID"
    OS_VERSION="${VERSION_ID:-}"

    # Debian Sid quirk
    if [[ $OS_ID == "debian" ]] && [[ "$OS_VERSION" == "" ]]; then
        OS_VERSION="sid"
    fi
}

_installPackages()
{
    # NOTICE: Keep this code in sync in the following files:
    #   - dev-tools/install-dev-software.sh
    #   - install/install-snoopy.sh
    #
    # Expects:
    #   - Global variable OS_ID
    #   - Global variable OS_VERSION
    #   - Global variable PACKAGE_NAMES_ARCH
    #   - Global variable PACKAGE_NAMES_DEBIAN
    #   - Global variable PACKAGE_NAMES_REDHAT
    #   - Global variable PACKAGE_NAMES_SUSE
    #
    # Sets:
    #   - (nothing)
    #
    # Returns:
    #   - false on error
    USE_SUDO="sudo -n"
    MY_UID=`id -u`
    if [ "$MY_UID" == "0" ]; then
        USE_SUDO=""
    fi

    case "$OS_ID" in
        arch)
            $USE_SUDO sudo pacman -Syu --noconfirm $PACKAGE_NAMES_ARCH
            ;;

        debian|ubuntu)
            DEBIAN_FRONTEND="noninteractive" $USE_SUDO apt-get update -y
            DEBIAN_FRONTEND="noninteractive" $USE_SUDO apt-get install -y $PACKAGE_NAMES_DEBIAN
            ;;

        rhel|centos|almalinux)
            $USE_SUDO yum install -y --allowerasing $PACKAGE_NAMES_REDHAT
            ;;

        sles|opensuse-leap|opensuse-tumbleweed)
            $USE_SUDO zypper -n install $PACKAGE_NAMES_SUSE
            ;;

        *)
            _fatalError "Unknown OS: '$OS_ID'. Install the following programs manually: $PACKAGE_NAMES_DEBIAN"
            ;;
    esac
}



# Now run the OS detection
#
OS_ID=""
OS_VERSION=""
_detectOperatingSystem
if [ "$OS_ID" == "" ]; then
    _fatalError "Unable to detect your OS via /etc/os-release. Install the following programs manually: $PROGRAM_NAMES"
fi



### Now install the packages
#
_installPackages
