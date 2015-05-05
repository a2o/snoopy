#!/bin/bash



### Signal success
#
set +x
echo -e "\nStarting AutoTools run:\n"



### Shell setup
#
# Treat all errors as fatal and trace the progress
#
set -e
set -x



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



### Run autotools
#
#libtoolize --install --copy --force
#aclocal
#automake --add-missing --copy
#autoconf
#autoreconf -i   # This is the new recommended way
autoreconf -i -v



### Remove stale/backup files
#
rm -f config.h.in~



### Signal success and exit
#
set +x
echo -e "\nAutoTools run was completed sucessfully. You should run ./configure now.\n"
