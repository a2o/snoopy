#!/bin/bash


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


### Run autotools
#
aclocal
#automake --add-missing --copy
autoconf


### Signal success
#
echo "autogen.sh completed sucessfully."
