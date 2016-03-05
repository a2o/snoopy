#!/bin/bash



### Shell configuration
#
set -e
set -u
set -o pipefail



### Configuration
#
SNOOPY_GIT_ORIGIN_URI="https://github.com/a2o/snoopy.git"
SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX="http://source.a2o.si/download/snoopy"
SNOOPY_INSTALL_LOGFILE="`pwd`/snoopy-install.log"
SNOOPY_TRAVIS_BUILD=${SNOOPY_TRAVIS_BUILD:-false}



### Help method
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



### Install distro-dependent build prerequisites, if missing
#
# 'socat' is not strictly needed. It is used by test suite, and is thus required
# by autotools too.
# If you remove it from this list, remove it from configure.ac too, or installations
# performed by this script will fail.
#
REQUIRED_PROGRAMS="gcc gzip make socat tar wget"
if [ "$SNOOPY_SOURCE_TYPE" == "git" ]; then
    REQUIRED_PROGRAMS_GITINSTALL="autoconf git libtool m4"
    REQUIRED_PROGRAMS="$REQUIRED_PROGRAMS $REQUIRED_PROGRAMS_GITINSTALL"
fi

if which $REQUIRED_PROGRAMS &> /dev/null; then
    echo "SNOOPY INSTALL: Required programs already present: $REQUIRED_PROGRAMS"
else
    if [ "$SNOOPY_INSTALL_RUNNING_AS_ROOT" != "true" ]; then
        echo "SNOOPY INSTALL: Unable to run package installation, not root"
    else
        echo "SNOOPY INSTALL: Installing distro-specific packages for programs: $REQUIRED_PROGRAMS"
        if [ -f /etc/debian_version ]; then
            # Debian, Ubuntu
            # About /dev/null: http://askubuntu.com/questions/372810/how-to-prevent-script-not-to-stop-after-apt-get
            apt-get -y install $REQUIRED_PROGRAMS < "/dev/null"
        elif [ -f /etc/redhat-release ]; then
            # RHEL, Fedora, CentOS
            yum install -y $REQUIRED_PROGRAMS
        fi
    fi

    # Check again
    for PROGRAM in `echo "which $REQUIRED_PROGRAMS"`; do
        if ! which $PROGRAM &> /dev/null; then
            echo "SNOOPY INSTALL ERROR: '$PROGRAM' program not found!"
            echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
            exit 1
        fi
    done
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
    SNOOPY_LOCAL_GIT_DIR="snoopy-install-from.git"

    rm -rf ./$SNOOPY_LOCAL_GIT_DIR
    git clone $SNOOPY_GIT_ORIGIN_URI $SNOOPY_LOCAL_GIT_DIR >> $SNOOPY_INSTALL_LOGFILE 2>&1
    cd $SNOOPY_LOCAL_GIT_DIR

    echo "SNOOPY INSTALL: Checking out git ref: $SNOOPY_GIT_REF_TO_INSTALL" | tee -a $SNOOPY_INSTALL_LOGFILE
    git checkout $SNOOPY_GIT_REF_TO_INSTALL >> $SNOOPY_INSTALL_LOGFILE 2>&1

    # Submodules are in use since iniparser was moved to own git repo
    if [ -f .gitmodules ]; then
        echo "SNOOPY INSTALL: Git submodules: initilizing... " | tee -a $SNOOPY_INSTALL_LOGFILE
        git submodule init     >> $SNOOPY_INSTALL_LOGFILE 2>&1
        echo "SNOOPY INSTALL: Git submodules: updating... "    | tee -a $SNOOPY_INSTALL_LOGFILE
        git submodule update   >> $SNOOPY_INSTALL_LOGFILE 2>&1
    fi

    echo -n "SNOOPY INSTALL: Bootstraping build environment... " | tee -a $SNOOPY_INSTALL_LOGFILE
    if [ -x bootstrap.sh ]; then
        ./bootstrap.sh         >> $SNOOPY_INSTALL_LOGFILE 2>&1

    elif [ -x autogen.sh ]; then
        # Run these two first, to avoid errors (git-snoopy-2.2.6 for example)
        aclocal                >> $SNOOPY_INSTALL_LOGFILE 2>&1
        automake --add-missing >> $SNOOPY_INSTALL_LOGFILE 2>&1

        ./autogen.sh           >> $SNOOPY_INSTALL_LOGFILE 2>&1

    elif [ -x configure ]; then
        # Do nothing, ./configure is commited
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
        SNOOPY_VERSION_TO_INSTALL=`wget -q -O - $SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX/snoopy-latest-version.txt`
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
        SNOOPY_PACKAGE_URI="$SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX/$SNOOPY_PACKAGE_FILENAME"
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
    echo "SNOOPY LOGGER is now installed and enabled."
    echo
    echo "TIP #1: If Snoopy is to be enabled for all processes, you need"
    echo "        to restart your system, or at least all services on it."
    echo
    echo "TIP #2: If you ever need to disable Snoopy, you should use provided"
    echo "        'snoopy-disable' script. Use 'snoopy-enable' to reenable it."
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
