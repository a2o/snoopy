#!/bin/bash



### Shell setup
#
# Treat all errors as fatal and trace the progress
#
set -e
set -u



### Check if autoconf is present on the system
#
if ! command -v autoreconf > /dev/null; then
    echo "ERROR: Program 'autoreconf' not found. Consider running './dev-tools/install-dev-software.sh' to correct this."
    exit 1
fi



### Check autotools version
#
RES=`autoreconf --version | head -n1 | cut -d' ' -f4 | sed -e 's/\.//'`
if [ "$RES" -lt "268" ]; then
    echo "ERROR: Your autotools version is too old. At least version 2.68 is required."
    exit 1
fi



### Clear cache
#
if [ -d autom4te.cache ]; then
    rm -rf autom4te.cache
fi



### Bug in aclocal: manually create m4 directory if it does not exist
#
if [ ! -d build/m4 ]; then
    mkdir -p build/m4
fi



### Signal process start
#
echo "###"
echo "### Starting AutoTools run:"
echo "###"
echo



### Run autotools
#
autoreconf -i -v



### Remove stale/backup files
#
rm -f config.h.in~



### Signal success and exit
#
echo
echo "###"
echo "### AutoTools run was completed successfully."
echo "### You should run ./configure now."
echo "###"
echo
