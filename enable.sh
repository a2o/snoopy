#!/bin/sh

# A script that enables snoopy logging for system wide installation.
# You have to run it as privileged user.
# $Id: enable.sh 19 2010-02-10 01:45:48Z bostjanskufca $



### Where is the library installed?
LIBDIR=$1
if [ "x$LIBDIR" == "x" ]; then
    echo "ERROR: No argument given, libdir expected"
    exit 1
fi



### Check snoopy installation
if [ ! -x $LIBDIR/snoopy.so ]; then
    echo "ERROR: $LIBDIR/snoopy.so is not installed or is not executable"
    exit 1
fi



### Check user that is running this script
touch /etc/ld.so.preload
if [ ! -w /etc/ld.so.preload ]; then
    echo 'ERROR: /etc/ld.so.preload not writable - are you running this script as a unprivileged user?'
    exit 1
fi



### Do the actual install
if [ `grep -c "^$LIBDIR/snoopy.so"   /etc/ld.so.preload` == 0 ]; then
    echo "$LIBDIR/snoopy.so" >> /etc/ld.so.preload
    echo "Snoopy enabled in /etc/ld.so.preload. Check syslog messages for output."
else
    echo "Snoopy already enabled in /etc/ld.so.preload. Check syslog messages for output."
fi
