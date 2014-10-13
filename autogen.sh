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



### Run autotools
#
#libtoolize --install --copy --force
#aclocal
#automake --add-missing --copy
#autoconf
autoreconf   # This is the new recommended way



### Signal success and exit
#
set +x
echo -e "\nAutoTools run was completed sucessfully. You should run ./configure now.\n"
