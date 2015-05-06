#!/bin/bash



### Configuration
#
SNOOPY_DOWNLOAD_URI_PREFIX="http://source.a2o.si/download/snoopy"
SNOOPY_INSTALL_LOGFILE="`pwd`/snoopy-install.log"



### Check if running as root
#
if [ "`id -u`" != "0" ]; then
    echo "SNOOPY INSTALL ERROR: This installation must be run as root."
    echo "Hint: 'sudo COMMAND' perhaps?"
    exit 1
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



### Get last stable Snoopy release
#
echo -n "SNOOPY INSTALL: Getting latest Snoopy version: "
SNOOPY_PACKAGE_FILENAME=`wget -q -O - $SNOOPY_DOWNLOAD_URI_PREFIX/snoopy-latest-package-filename.txt`
SNOOPY_PACKAGE_DIRNAME=`echo "$SNOOPY_PACKAGE_FILENAME" | sed -e 's/\.tar.gz$//'`
echo "$SNOOPY_PACKAGE_FILENAME"



### Download Snoopy package
#
echo -n "SNOOPY INSTALL: Downloading $SNOOPY_DOWNLOAD_URI_PREFIX/$SNOOPY_PACKAGE_FILENAME... "
rm -f $SNOOPY_PACKAGE_FILENAME
wget $SNOOPY_DOWNLOAD_URI_PREFIX/$SNOOPY_PACKAGE_FILENAME >> $SNOOPY_INSTALL_LOGFILE 2>&1
echo "done."



### Untar, build and configure
#
echo -n "SNOOPY INSTALL: Unpacking $SNOOPY_PACKAGE_FILENAME... "
rm -rf $SNOOPY_PACKAGE_DIRNAME
tar -xzf $SNOOPY_PACKAGE_FILENAME
cd $SNOOPY_PACKAGE_DIRNAME
echo "done."



echo -n "SNOOPY INSTALL: Configuring... "
./configure --enable-config-file --sysconfdir=/etc --enable-filtering >> $SNOOPY_INSTALL_LOGFILE 2>&1
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
