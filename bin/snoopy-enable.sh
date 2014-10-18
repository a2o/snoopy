#!/bin/sh



# This is a script that enables snoopy system-wide.
# You have to run this script as a privileged user.
# This script is also run when you execute "make enable".



### Where is the library installed?
#
LIBDIR=$1
if [ "x$LIBDIR" = "x" ]; then
    echo "ERROR: No argument given, libdir expected"
    exit 1
fi



### Check snoopy installation
#
if [ ! -x $LIBDIR/libsnoopy.so ]; then
    echo
    echo "ERROR: $LIBDIR/libsnoopy.so is not installed or is not executable"
    echo
    exit 1
fi



### Check user that is running this script
#
touch /etc/ld.so.preload
if [ ! -w /etc/ld.so.preload ]; then
    echo
    echo 'ERROR: /etc/ld.so.preload not writable - are you running this script as a unprivileged user?'
    echo
    exit 1
fi



### Do the actual installation
#
echo "ENABLE:"
COUNT=`grep -Ec "/(lib)?snoopy.so" /etc/ld.so.preload`
if [ "$COUNT" -gt "1" ]; then
    echo
    echo "ERROR: Multiple instances of (lib)snoopy.so found in /etc/ld.so.preload. Unable to proceed."
    echo
    exit 1
elif [ "$COUNT" -eq "1" ]; then
    echo -n "ENABLE:   Removing from /etc/ld.so.preload: "
    cat /etc/ld.so.preload | grep -E "/(lib)?snoopy.so"
    sed -i "/\/\(lib\)\?snoopy.so/d" /etc/ld.so.preload
fi
echo "ENABLE:   Adding to /etc/ld.so.preload:     $LIBDIR/libsnoopy.so"
echo "$LIBDIR/libsnoopy.so" >> /etc/ld.so.preload
echo "ENABLE:"
echo "ENABLE:   SUCCESS!"
echo "ENABLE:"
echo "ENABLE:   Snoopy is now added to /etc/ld.so.preload, which means it is enabled."
echo "ENABLE:   Check your syslog files for output."
echo "ENABLE:"
