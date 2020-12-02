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
. `dirname $0`/_bootstrap.sh



### Definitions
#
# NOTICE: Certain changes here must potentially be reflected
# in the ../install/install-snoopy.sh file too.
#
       PROGRAM_NAMES="autoconf curl gcc git gzip hostname  libtoolize m4 make ps     socat tar wget"
  PACKAGE_NAMES_ARCH="autoconf curl gcc git gzip inetutils libtool    m4 make procps socat tar wget"
PACKAGE_NAMES_DEBIAN="autoconf curl gcc git gzip           libtool    m4 make procps socat tar wget"
PACKAGE_NAMES_REDHAT="autoconf curl gcc git gzip           libtool    m4 make procps socat tar wget"
  PACKAGE_NAMES_SUSE="autoconf curl gcc git gzip hostname  libtool    m4 make procps socat tar wget"



### Check for presence
#
_areAllRequiredProgramsPresent()
{
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

if _areAllRequiredProgramsPresent "$PROGRAM_NAMES"; then
    _echo "All required programs are already installed, nice."
    exit
fi



### Detect OS
#
OS_ID=""
OS_VERSION=""

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS_ID="$ID"
    OS_VERSION="${VERSION_ID:-}"

    # Debian Sid quirk
    if [[ $OS_ID == "debian" ]] && [[ "$OS_VERSION" == "" ]]; then
        OS_VERSION="sid"
    fi

else
    # Try to detect RHEL/CentOS 6
    if [ -f /etc/redhat-release ]; then
        if fgrep "CentOS release 6." /etc/redhat-release > /dev/null; then
            OS_ID="centos"
            OS_VERSION="6"
        elif fgrep "Red Hat Enterprise Linux Server release 6." /etc/redhat-release > /dev/null; then
            OS_ID="rhel"
            OS_VERSION="6"
        fi
    fi
fi

if [ "$OS_ID" == "" ]; then
    _fatalError "Unable to detect your OS via /etc/os-release. Install the following programs manually: $PROGRAM_NAMES"
fi



### Install required packages
#
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

    rhel|centos)
        #For v6, for what? TODO
        #yum install -y epel-release
        $USE_SUDO yum install -y $PACKAGE_NAMES_REDHAT
        ;;


    sles|opensuse-leap|opensuse-tumbleweed)
        $USE_SUDO zypper -n install $PACKAGE_NAMES_SUSE
        ;;

    *)
        _fatalError "Unknown OS: '$OS_ID'. Install the following programs manually: $PROGRAM_NAMES"
esac



### Recheck
#
if ! _areAllRequiredProgramsPresent "$PROGRAM_NAMES"; then
    _fatalError "Even after installation, at least some of the required programs are still not available. Seems like a bug in this script."
else
    _echo "All required programs are now installed."
fi
