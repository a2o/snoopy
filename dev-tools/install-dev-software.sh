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
# NOTICE: Certain changes here must potentially be reflected
# in the ../install/install-snoopy.sh file too.
#
       PROGRAM_NAMES="autoconf aclocal  curl diff      file find      gcc git gzip hostname  libtoolize m4 make ps     socat tar wget"
PACKAGE_NAMES_ALPINE="autoconf automake curl diffutils file           gcc git gzip           libtool    m4 make procps socat tar wget alpine-sdk"
  PACKAGE_NAMES_ARCH="autoconf automake curl diffutils file           gcc git gzip inetutils libtool    m4 make procps socat tar wget"
PACKAGE_NAMES_DEBIAN="autoconf automake curl diffutils file           gcc git gzip           libtool    m4 make procps socat tar wget"
PACKAGE_NAMES_REDHAT="autoconf automake curl diffutils file           gcc git gzip hostname  libtool    m4 make procps socat tar wget"
  PACKAGE_NAMES_SUSE="autoconf automake curl diffutils file findutils gcc git gzip hostname  libtool    m4 make procps socat tar wget"



### Software check & install functions
#
# NOTICE: Keep this code in sync in the following files:
#   - dev-tools/install-dev-software.sh
#   - install/install-snoopy.sh
#
_areAllRequiredProgramsPresent()
{
    # NOTICE: Keep this code in sync in the following files:
    #   - dev-tools/install-dev-software.sh
    #   - install/install-snoopy.sh
    REQUIRED_PROGRAMS="$1"

    ALL_REQUIRED_PROGRAMS_PRESENT="true"
    for REQUIRED_PROGRAM in $REQUIRED_PROGRAMS; do
        if ! command -v $REQUIRED_PROGRAM > /dev/null; then
            ALL_REQUIRED_PROGRAMS_PRESENT="false"
            _echo "The following program is missing: $REQUIRED_PROGRAM"
        fi
    done

    if [ "$ALL_REQUIRED_PROGRAMS_PRESENT" == "true" ]; then
        true
    else
        false
    fi
}

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
    #   - Global variable PACKAGE_NAMES_ALPINE
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
        alpine)
            $USE_SUDO apk add $PACKAGE_NAMES_ALPINE
            ;;

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



### Check programs presence
#
if _areAllRequiredProgramsPresent "$PROGRAM_NAMES"; then
    _echo "All required programs are already installed, nice."
    exit
fi


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



### Recheck
#
if ! _areAllRequiredProgramsPresent "$PROGRAM_NAMES"; then
    _fatalError "Even after installation, at least some of the required programs are still not available. Seems like a bug in this script."
else
    _echo "All required programs are now installed."
fi
