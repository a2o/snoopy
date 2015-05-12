#!/bin/bash



### Configuration
#
SNOOPY_GIT_ORIGIN_URI="https://github.com/a2o/snoopy.git"
SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX="http://source.a2o.si/download/snoopy"
SNOOPY_INSTALL_LOGFILE="`pwd`/snoopy-install.log"



### What to install?
#
ARG_INSTALL_MODE="$1"
case $ARG_INSTALL_MODE in

    git-*)
        SNOOPY_INSTALL_MODE="git"
        SNOOPY_GIT_REF_TO_INSTALL=`echo "$ARG_INSTALL_MODE" | sed -e 's/^git-//'`
        ;;

    latest-stable|stable|stable-latest)
        SNOOPY_INSTALL_MODE="package-latest-stable";
        ;;

#    latest-preview|preview|preview-latest)
#        SNOOPY_INSTALL_MODE="package-latest-preview";
#        ;;

    [1-9].[0-9]*.[0-9]*)
        SNOOPY_INSTALL_MODE="package-exact-version"
        SNOOPY_VERSION_TO_INSTALL="$ARG_INSTALL_MODE"
        ;;
    *)
        echo "SNOOPY INSTALL ERROR: Unknown installation mode."
        echo
        echo "Possible options:"
        echo "- 'stable'    ; installs latest stable version"
#        echo "- 'preview'   ; installs latest preview version, if available"
        echo "- 'X.Y.Z'     ; installs specific version from release package"
        echo "- 'git-REF'   ; install directly from git, where REF is either:"
        echo "     - a branch name,"
        echo "     - tag or"
        echo "     - commit SHA hash."
        echo
        exit 1
        ;;
esac



### Check if running as root
#
if [ "`id -u`" != "0" ]; then
    echo "SNOOPY INSTALL ERROR: This installation must be run as root."
    echo "Hint: 'sudo COMMAND' perhaps?"
#    exit 1
fi



### Install distro-dependent build prerequisites
#
echo "SNOOPY INSTALL: Installing distro-specific packages 'gcc' and 'make'..."
if [ -f /etc/debian_version ]; then
    # Debian, Ubuntu
    # About /dev/null: http://askubuntu.com/questions/372810/how-to-prevent-script-not-to-stop-after-apt-get
    apt-get -y install gcc gzip make tar wget < "/dev/null"
elif [ -f /etc/redhat-release ]; then
    # RHEL, Fedora, CentOS
    yum install -y gcc gzip make tar wget
else
    # Check if gcc and make are present
    if ! which which &> /dev/null; then
        echo "SNOOPY INSTALL ERROR: 'which' program not found!"
        echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
        exit 1
    fi

    if ! which gcc &> /dev/null; then
        echo "SNOOPY INSTALL ERROR: GCC compiler not found!"
        echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
        exit 1
    fi

    if ! which make &> /dev/null; then
        echo "SNOOPY INSTALL ERROR: 'make' program not found!"
        echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
        exit 1
    fi

    if ! which tar &> /dev/null; then
        echo "SNOOPY INSTALL ERROR: 'tar' program not found!"
        echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
        exit 1
    fi

    if ! which gzip &> /dev/null; then
        echo "SNOOPY INSTALL ERROR: 'gzip' program not found!"
        echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
        exit 1
    fi
fi



### If building from git, check for some more stuff
#
if [ "$SNOOPY_INSTALL_MODE" == "git" ]; then
    echo "SNOOPY INSTALL: Installing distro-specific packages 'autoconf', 'git', 'm4' and others..."
    if [ -f /etc/debian_version ]; then
        # Debian, Ubuntu
        # About /dev/null: http://askubuntu.com/questions/372810/how-to-prevent-script-not-to-stop-after-apt-get
        apt-get -y install autoconf git < "/dev/null"
    elif [ -f /etc/redhat-release ]; then
        # RHEL, Fedora, CentOS
        yum install -y autoconf git
    else
        # Check manually
        if ! which autoreconf &> /dev/null; then
            echo "SNOOPY INSTALL ERROR: 'autoreconf' not found!"
            echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
            exit 1
        fi

        # Check manually
        if ! which git &> /dev/null; then
            echo "SNOOPY INSTALL ERROR: 'git' not found!"
            echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
            exit 1
        fi

        # Check manually
        if ! which m4 &> /dev/null; then
            echo "SNOOPY INSTALL ERROR: 'm4' not found!"
            echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
            exit 1
        fi


    if ! which gzip &> /dev/null; then
        echo "SNOOPY INSTALL ERROR: 'gzip' program not found!"
        echo "SNOOPY INSTALL ERROR: Install it and rerun this installer."
        exit 1
    fi
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
echo "SNOOPY INSTALL: Starting installation, log file name: $SNOOPY_INSTALL_LOGFILE"
echo "SNOOPY INSTALL: Installation mode: $SNOOPY_INSTALL_MODE"



### Obtain source code
#
if [ "$SNOOPY_INSTALL_MODE" == "git" ]; then

    echo "SNOOPY INSTALL: Cloning git repository: $SNOOPY_GIT_ORIGIN_URI"
    SNOOPY_LOCAL_GIT_DIR="snoopy-install-from.git"

    rm -rf ./$SNOOPY_LOCAL_GIT_DIR
    git clone $SNOOPY_GIT_ORIGIN_URI $SNOOPY_LOCAL_GIT_DIR >> $SNOOPY_INSTALL_LOGFILE 2>&1
    cd $SNOOPY_LOCAL_GIT_DIR\

    echo "SNOOPY INSTALL: Checking out git ref: $SNOOPY_GIT_REF_TO_INSTALL"
    git checkout $SNOOPY_GIT_REF_TO_INSTALL >> $SNOOPY_INSTALL_LOGFILE 2>&1

    echo "SNOOPY INSTALL: Bootstraping installation procedure (autoreconf etc.)..."
    if [ -x bootstrap.sh ]; then
        ./bootstrap.sh         >> $SNOOPY_INSTALL_LOGFILE 2>&1

    elif [ -x autogen.sh ]; then
        # Run these two first, to avoid errors (git-snoopy-2.2.6 for example)
        aclocal                >> $SNOOPY_INSTALL_LOGFILE 2>&1
        automake --add-missing >> $SNOOPY_INSTALL_LOGFILE 2>&1

        ./autogen.sh           >> $SNOOPY_INSTALL_LOGFILE 2>&1

    else
        echo "SNOOPY INSTALL ERROR: This git ref is too old to be supported by this installation procedure."
        echo "SNOOPY INSTALL ERROR: You will have to install it manually."
        exit 1
    fi

else

    ### Determine version to install
    #
    if [ "$SNOOPY_INSTALL_MODE" == "package-latest-stable" ]; then
        echo -n "SNOOPY INSTALL: Getting latest Snoopy version: "
        SNOOPY_PACKAGE_VERSION=`wget -q -O - $SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX/snoopy-latest-version.txt`
        SNOOPY_PACKAGE_FILENAME=`wget -q -O - $SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX/snoopy-latest-package-filename.txt`
    fi

    if [ "$SNOOPY_INSTALL_MODE" == "package-exact-version" ]; then
        echo -n "SNOOPY INSTALL: Will install the following package: "
        SNOOPY_PACKAGE_VERSION="$SNOOPY_VERSION_TO_INSTALL"
        SNOOPY_PACKAGE_FILENAME="snoopy-$SNOOPY_PACKAGE_VERSION.tar.gz"
    fi
    SNOOPY_PACKAGE_URI="$SNOOPY_PACKAGE_DOWNLOAD_URI_PREFIX/$SNOOPY_PACKAGE_FILENAME"
    SNOOPY_PACKAGE_DIRNAME=`echo "$SNOOPY_PACKAGE_FILENAME" | sed -e 's/\.tar.gz$//'`
    echo "$SNOOPY_PACKAGE_FILENAME"

    ### Download Snoopy package
    #
    echo -n "SNOOPY INSTALL: Downloading $SNOOPY_PACKAGE_URI... "
    rm -f $SNOOPY_PACKAGE_FILENAME
    wget $SNOOPY_PACKAGE_URI >> $SNOOPY_INSTALL_LOGFILE 2>&1
    echo "done."

    ### Untar, build and configure
    #
    echo -n "SNOOPY INSTALL: Unpacking $SNOOPY_PACKAGE_FILENAME... "
    rm -rf $SNOOPY_PACKAGE_DIRNAME
    tar -xzf $SNOOPY_PACKAGE_FILENAME
    cd $SNOOPY_PACKAGE_DIRNAME
    echo "done."
fi



### Configure, build, install, enable
#

# Which configure flag is the right one
#
if ./configure --help | grep enable-filtering > /dev/null; then
    SNOOPY_INSTALL_CONFIGURE_FLAG_FILTERING="--enable-filtering"
else
    SNOOPY_INSTALL_CONFIGURE_FLAG_FILTERING="--enable-filter"   # Older variation
fi

echo -n "SNOOPY INSTALL: Configuring... "
./configure \
    --enable-config-file \
    --sysconfdir=/etc \
    $SNOOPY_INSTALL_CONFIGURE_FLAG_FILTERING \
    >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done."

echo -n "SNOOPY INSTALL: Building... "
make         >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done."

echo -n "SNOOPY INSTALL: Installing... "
make install >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done."

echo -n "SNOOPY INSTALL: Enabling... "
make enable  >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done."



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



### End bash subshell
#
)
if [ "$?" != "0" ]; then
    echo
    echo
    echo "Last 5 lines of Snoopy installation log file:"
    tail -n5 $SNOOPY_INSTALL_LOGFILE
    echo
    echo "SNOOPY INSTALL ERROR: Something weird happened!"
    echo "SNOOPY INSTALL ERROR: Please inspect log file for details ($SNOOPY_INSTALL_LOGFILE)"
    exit 1
fi



### All done
#
true
