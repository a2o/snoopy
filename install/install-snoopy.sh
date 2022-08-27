#!/bin/bash



### Shell configuration
#
set -e
set -u
set -o pipefail



### Configuration
#
SNOOPY_GIT_ORIGIN_URI="https://github.com/a2o/snoopy.git"
SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX="https://github.com/a2o/snoopy/releases/download"
SNOOPY_INSTALL_LOGFILE="`pwd`/install-snoopy.log"
SNOOPY_TRAVIS_BUILD=${SNOOPY_TRAVIS_BUILD:-false}



### Helper functions
#
function _snoopy_install_showHelp()
{
    echo "Possible installation arguments/modes:"
    echo "- 'stable'    ; installs latest stable version"
    echo "- 'preview'   ; installs latest preview version (defaults to git-master most of the time)"
    echo "- 'X.Y.Z'     ; installs specific version from release package"
    echo "- 'git-REF'   ; install directly from git, where REF is either:"
    echo "     - branch name,"
    echo "     - tag,"
    echo "     - commit SHA hash."
    echo "- 'path/to/snoopy-X.Y.Z.tar.gz'     ; installs specific pre-downloaded Snoopy release package"
    echo "- 'download'  ; only downloads latest Snoopy release package"
}

_echo()
{
    echo "SNOOPY_INSTALL: ${1:-}"
}

_fatalError() {
    ERR_FILE="$0"
    ERR_MSG="$1"
    echo "SNOOPY INSTALL ERROR: $ERR_MSG" 1>&2
    exit 1
}



### What to install?
#
ARG_INSTALL_MODE="${1:-unspecified}"
case $ARG_INSTALL_MODE in

    git-*)
        SNOOPY_INSTALL_MODE="git-REFERENCE"
        SNOOPY_SOURCE_TYPE="git"
        SNOOPY_DOWNLOAD_MODE="git-clone"
        SNOOPY_GIT_REF_TO_INSTALL=`echo "$ARG_INSTALL_MODE" | sed -e 's/^git-//'`
        ;;

    latest-preview|preview|preview-latest)
        SNOOPY_INSTALL_MODE="git-latest-preview"
        SNOOPY_SOURCE_TYPE="git"
        SNOOPY_DOWNLOAD_MODE="git-clone"
        SNOOPY_GIT_REF_TO_INSTALL="master"
        ;;

    latest-stable|stable|stable-latest|latest)
        SNOOPY_INSTALL_MODE="package-latest-stable"
        SNOOPY_SOURCE_TYPE="package"
        SNOOPY_DOWNLOAD_MODE="package-download"
        SNOOPY_PACKAGE_DOWNLOAD="true"
        SNOOPY_VERSION_TO_INSTALL="latest"
        ;;

    download|download-only)
        SNOOPY_INSTALL_MODE="download-only"
        SNOOPY_SOURCE_TYPE="package"
        SNOOPY_DOWNLOAD_MODE="package-download"
        SNOOPY_PACKAGE_DOWNLOAD="true"
        SNOOPY_VERSION_TO_INSTALL="latest"
        ;;

    [1-9].[0-9]*.[0-9]*)
        SNOOPY_INSTALL_MODE="package-specific-version"
        SNOOPY_SOURCE_TYPE="package"
        SNOOPY_DOWNLOAD_MODE="package-download"
        SNOOPY_PACKAGE_DOWNLOAD="true"
        SNOOPY_VERSION_TO_INSTALL="$ARG_INSTALL_MODE"
        ;;

    *)
        # Check if file name/path has been passed - perform a local install
        if [[ $ARG_INSTALL_MODE =~ snoopy-[-_.0-9a-zA-Z]+\.tar\.gz$ ]] && [[ -f $ARG_INSTALL_MODE ]]; then

            SNOOPY_INSTALL_MODE="local-package-file"
            SNOOPY_SOURCE_TYPE="package"
            SNOOPY_DOWNLOAD_MODE="package-download"
            SNOOPY_PACKAGE_DOWNLOAD="false"
            SNOOPY_PACKAGE_PATH="$ARG_INSTALL_MODE"
            SNOOPY_VERSION_TO_INSTALL=`basename $SNOOPY_PACKAGE_PATH | sed -e 's/^snoopy-//' | sed -e 's/.tar.gz$//'`

        else

            echo
            echo "SNOOPY INSTALL ERROR: Unknown installation mode."
            echo
            _snoopy_install_showHelp
            echo
            exit 1

        fi
        ;;
esac



### Check if running as root
#
SNOOPY_INSTALL_RUNNING_AS_ROOT="true"
if [ "`id -u`" != "0" ]; then
    SNOOPY_INSTALL_RUNNING_AS_ROOT="false"
    echo "SNOOPY INSTALL ERROR: This installation must be run as root."
    echo "Hint: 'sudo COMMAND' perhaps?"
    if [ "$SNOOPY_TRAVIS_BUILD" == "true" ]; then
        echo "SNOOPY INSTALL: Ignoring error above, we are running inside Travis CI."
    else
        exit 1
    fi
fi



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
            $USE_SUDO yum install -y $PACKAGE_NAMES_REDHAT
            ;;

        sles|opensuse-leap|opensuse-tumbleweed)
            $USE_SUDO zypper -n install $PACKAGE_NAMES_SUSE
            ;;

        *)
            _fatalError "Unknown OS: '$OS_ID'. Install the following programs manually: $PACKAGE_NAMES_DEBIAN"
            ;;
    esac
}



### Install distro-dependent build prerequisites, if missing
#
# Since running a test suite has been removed from this script,
# we don't look for 'ps' and 'socat' programs anymore.
#
# NOTICE: Certain changes here must potentially be reflected
# in the ../dev-tools/install-dev-software.sh file too.
#
# NOTICE: Snoopy releases 2.4.10 and earlier actually _require_ `socat` and `ps`
# to be present for the `./configure` step to succeed. Let's keep this here for
# some time (at least until >2.4.10 is released).
#
#

       PROGRAM_NAMES="find      gcc gzip make ps     socat tar wget"
PACKAGE_NAMES_ALPINE="          gcc gzip make procps socat tar wget alpine-sdk"
  PACKAGE_NAMES_ARCH="          gcc gzip make procps socat tar wget"
PACKAGE_NAMES_DEBIAN="          gcc gzip make procps socat tar wget"
PACKAGE_NAMES_REDHAT="          gcc gzip make procps socat tar wget"
  PACKAGE_NAMES_SUSE="findutils gcc gzip make procps socat tar wget"
if [ "$SNOOPY_SOURCE_TYPE" == "git" ]; then
           PROGRAM_NAMES="autoconf aclocal  curl find      gcc git gzip hostname  libtoolize m4 make ps     socat tar wget"
    PACKAGE_NAMES_ALPINE="autoconf automake curl           gcc git gzip           libtool    m4 make procps socat tar wget alpine-sdk"
      PACKAGE_NAMES_ARCH="autoconf automake curl           gcc git gzip inetutils libtool    m4 make procps socat tar wget"
    PACKAGE_NAMES_DEBIAN="autoconf automake curl           gcc git gzip           libtool    m4 make procps socat tar wget"
    PACKAGE_NAMES_REDHAT="autoconf automake curl           gcc git gzip hostname  libtool    m4 make procps socat tar wget"
      PACKAGE_NAMES_SUSE="autoconf automake curl findutils gcc git gzip hostname  libtool    m4 make procps socat tar wget"
fi

if _areAllRequiredProgramsPresent "$PROGRAM_NAMES"; then
    echo "SNOOPY INSTALL: Required programs already present: $PROGRAM_NAMES"
else
    if [ "$SNOOPY_INSTALL_RUNNING_AS_ROOT" != "true" ]; then
        _fatalError "Unable to run package installation, not running as root"
    fi

    # Detect OS
    OS_ID=""
    OS_VERSION=""
    _detectOperatingSystem
    if [ "$OS_ID" == "" ]; then
        _fatalError "Unable to detect your OS via /etc/os-release. Install the following programs manually: $PROGRAM_NAMES"
    fi

    _installPackages

    # Check again
    if ! _areAllRequiredProgramsPresent "$PROGRAM_NAMES"; then
        echo "SNOOPY INSTALL ERROR: Even after installing it, the program above cannot be found."
        echo "SNOOPY INSTALL ERROR: Install it manually and rerun Snoopy installer."
        exit 1
    fi
fi





### Start bash subshell, to evaluate potential errors
#
(
set -e
set -u



### Starting installation
#
rm -f $SNOOPY_INSTALL_LOGFILE
touch $SNOOPY_INSTALL_LOGFILE
echo "SNOOPY INSTALL: Starting installation, log file: $SNOOPY_INSTALL_LOGFILE" | tee -a $SNOOPY_INSTALL_LOGFILE
echo "SNOOPY INSTALL: Installation mode: $SNOOPY_INSTALL_MODE"                  | tee -a $SNOOPY_INSTALL_LOGFILE



### Obtain source code
#
if [[ "$SNOOPY_SOURCE_TYPE" == "git" ]] && [[ "$SNOOPY_DOWNLOAD_MODE" == "git-clone" ]]; then

    echo "SNOOPY INSTALL: Cloning git repository: $SNOOPY_GIT_ORIGIN_URI" | tee -a $SNOOPY_INSTALL_LOGFILE
    SNOOPY_LOCAL_GIT_DIR="install-snoopy-git-repo"

    rm -rf ./$SNOOPY_LOCAL_GIT_DIR
    git clone $SNOOPY_GIT_ORIGIN_URI $SNOOPY_LOCAL_GIT_DIR >> $SNOOPY_INSTALL_LOGFILE 2>&1
    cd $SNOOPY_LOCAL_GIT_DIR

    echo "SNOOPY INSTALL: Checking out git ref: $SNOOPY_GIT_REF_TO_INSTALL" | tee -a $SNOOPY_INSTALL_LOGFILE
    git checkout $SNOOPY_GIT_REF_TO_INSTALL >> $SNOOPY_INSTALL_LOGFILE 2>&1

    echo -n "SNOOPY INSTALL: Bootstraping build environment... " | tee -a $SNOOPY_INSTALL_LOGFILE
    if [ -x bootstrap.sh ]; then
        ./bootstrap.sh         >> $SNOOPY_INSTALL_LOGFILE 2>&1

    elif [ -x autogen.sh ]; then
        # Run these two first, to avoid errors (git-snoopy-2.2.6 for example)
        aclocal                >> $SNOOPY_INSTALL_LOGFILE 2>&1
        automake --add-missing >> $SNOOPY_INSTALL_LOGFILE 2>&1

        ./autogen.sh           >> $SNOOPY_INSTALL_LOGFILE 2>&1

    elif [ -x configure ]; then
        # Do nothing, ./configure is committed
        true

    else
        echo "SNOOPY INSTALL ERROR: This git ref is too old to be supported by this installation procedure." | tee -a $SNOOPY_INSTALL_LOGFILE
        echo "SNOOPY INSTALL ERROR: You will have to install it manually." | tee -a $SNOOPY_INSTALL_LOGFILE
        exit 1
    fi
    echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE


elif [[ "$SNOOPY_SOURCE_TYPE" == "package" ]] && [[ "$SNOOPY_DOWNLOAD_MODE" == "package-download" ]]; then


    ### If 'latest', determine version to install
    #
    if [ "$SNOOPY_VERSION_TO_INSTALL" == "latest" ]; then
        echo -n "SNOOPY INSTALL: Getting latest Snoopy version... " | tee -a $SNOOPY_INSTALL_LOGFILE
        SNOOPY_VERSION_TO_INSTALL=`wget -q -O - https://raw.githubusercontent.com/a2o/snoopy/master/README.md | grep -E '^## Latest version$' -A10 | grep -E '^[|] +Stable +' | cut -d'|' -f3 | tr -d '[:space:]'`
        echo "got it, $SNOOPY_VERSION_TO_INSTALL" | tee -a $SNOOPY_INSTALL_LOGFILE
    else
        echo -n "SNOOPY INSTALL: Snoopy version to be installed... " | tee -a $SNOOPY_INSTALL_LOGFILE
        echo "$SNOOPY_VERSION_TO_INSTALL" | tee -a $SNOOPY_INSTALL_LOGFILE
    fi


    ### Determine version from local package
    #
    if [ "$SNOOPY_PACKAGE_DOWNLOAD" == "false" ]; then
        SNOOPY_PACKAGE_FILENAME=`basename $SNOOPY_PACKAGE_PATH`
    else
        SNOOPY_PACKAGE_FILENAME="snoopy-$SNOOPY_VERSION_TO_INSTALL.tar.gz"
    fi
    SNOOPY_PACKAGE_DIRNAME=`echo "$SNOOPY_PACKAGE_FILENAME" | sed -e 's/\.tar.gz$//'`
    SNOOPY_PACKAGE_VERSION=`echo $SNOOPY_PACKAGE_FILENAME | sed -e 's/^snoopy-//' | sed -e 's/.tar.gz$//'`


    ### Download Snoopy package
    #
    if [ "$SNOOPY_PACKAGE_DOWNLOAD" == "true" ]; then
        SNOOPY_GITHUB_RELEASE_NAME="snoopy-$SNOOPY_VERSION_TO_INSTALL"
        SNOOPY_PACKAGE_URI="$SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX/$SNOOPY_GITHUB_RELEASE_NAME/$SNOOPY_PACKAGE_FILENAME"
        echo -n "SNOOPY INSTALL: Downloading from $SNOOPY_PACKAGE_URI... " | tee -a $SNOOPY_INSTALL_LOGFILE
        rm -f $SNOOPY_PACKAGE_FILENAME
        wget $SNOOPY_PACKAGE_URI >> $SNOOPY_INSTALL_LOGFILE 2>&1
        echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE
        SNOOPY_PACKAGE_PATH="./$SNOOPY_PACKAGE_FILENAME"
    else
        echo -n "SNOOPY INSTALL: Will install the following local package: " | tee -a $SNOOPY_INSTALL_LOGFILE
        echo "$SNOOPY_PACKAGE_PATH" | tee -a $SNOOPY_INSTALL_LOGFILE
    fi


    ### Exit if in download-only mode
    #
    if [ "$SNOOPY_INSTALL_MODE" == "download-only" ]; then
        echo "SNOOPY INSTALL: Download-only mode detected, exiting." | tee -a $SNOOPY_INSTALL_LOGFILE
        exit 0
    fi


    ### Untar, build and configure
    #
    echo -n "SNOOPY INSTALL: Unpacking $SNOOPY_PACKAGE_PATH... " | tee -a $SNOOPY_INSTALL_LOGFILE
    rm -rf $SNOOPY_PACKAGE_DIRNAME
    tar -xzf $SNOOPY_PACKAGE_PATH
    cd $SNOOPY_PACKAGE_DIRNAME
    echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE


else

    echo -n "SNOOPY INSTALL ERROR: Internal error - undetermined download method" | tee -a $SNOOPY_INSTALL_LOGFILE
    exit 1

fi



###
### Configure, build, install, enable
###

# Detect travis
#
if [ "$SNOOPY_TRAVIS_BUILD" == "true" ]; then
    SNOOPY_INSTALL_CONFIGURE_PREFIX="--prefix=$HOME/usr/local"
    SNOOPY_INSTALL_CONFIGURE_SYSCONFDIR="--sysconfdir=$HOME/etc"
else
    SNOOPY_INSTALL_CONFIGURE_PREFIX=""
    SNOOPY_INSTALL_CONFIGURE_SYSCONFDIR="--sysconfdir=/etc"
fi

# Which configure flag is the right one
#
if ./configure --help | grep enable-filtering > /dev/null; then
    SNOOPY_INSTALL_CONFIGURE_FLAG_FILTERING="--enable-filtering"
else
    SNOOPY_INSTALL_CONFIGURE_FLAG_FILTERING="--enable-filter"   # Older variation
fi

echo -n "SNOOPY INSTALL: Configuring... " | tee -a $SNOOPY_INSTALL_LOGFILE
./configure \
    --enable-config-file \
    $SNOOPY_INSTALL_CONFIGURE_PREFIX \
    $SNOOPY_INSTALL_CONFIGURE_SYSCONFDIR \
    $SNOOPY_INSTALL_CONFIGURE_FLAG_FILTERING \
    >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE

echo -n "SNOOPY INSTALL: Building... " | tee -a $SNOOPY_INSTALL_LOGFILE
make         >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE

# Disabled for two reasons:
# - domain datasource was causing problems on misconfigured systems
# - combined tests are failing if snoopy is already enabled via /etc/ld.so.preload
#echo -n "SNOOPY INSTALL: Testing build... " | tee -a $SNOOPY_INSTALL_LOGFILE
#make check   >> $SNOOPY_INSTALL_LOGFILE 2>&1
#echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE

echo -n "SNOOPY INSTALL: Installing... " | tee -a $SNOOPY_INSTALL_LOGFILE
make install >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE

if [ "$SNOOPY_TRAVIS_BUILD" == "true" ]; then
    echo "SNOOPY INSTALL: NOT enabling, as we are not running as root (Travis-CI build)." | tee -a $SNOOPY_INSTALL_LOGFILE
else
    echo -n "SNOOPY INSTALL: Enabling... " | tee -a $SNOOPY_INSTALL_LOGFILE
    make enable  >> $SNOOPY_INSTALL_LOGFILE 2>&1
    echo "done." | tee -a $SNOOPY_INSTALL_LOGFILE



    ### Tell the user what to do next
    #
    echo
    echo "Snoopy Command Logger is now installed and enabled."
    echo
    echo "TIP #1: If Snoopy is to be enabled for all processes, you need"
    echo "        to restart your system, or at least all services on it."
    echo
    echo "TIP #2: If you ever need to disable Snoopy, use the provided"
    echo "        'snoopyctl disable' command. Use 'snoopyctl enable' to reenable it."
    echo
    echo "TIP #3: Snoopy output can usually be found somewhere in /var/log/*"
    echo "        Check your syslog configuration for details."
    echo
    echo "TIP #4: Configuration file location: /etc/snoopy.ini"
    echo "        See included comments for additional configuration options."
    echo
    echo "Snoopy wishes you a happy logging experience:)"
    echo
fi



### End bash subshell
#
)
if [ "$?" != "0" ]; then
    echo
    echo
    echo "Last 10 lines of Snoopy installation log file:"
    tail -n10 $SNOOPY_INSTALL_LOGFILE
    echo
    echo "SNOOPY INSTALL ERROR: Something weird happened!"
    echo "SNOOPY INSTALL ERROR: Please inspect log file for details ($SNOOPY_INSTALL_LOGFILE)"
    exit 1
fi



### All done
#
true
