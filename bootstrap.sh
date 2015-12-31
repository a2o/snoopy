#!/bin/bash



### Shell setup
#
# Treat all errors as fatal and trace the progress
#
set -e



### Check autotools version
#
RES=`autoreconf --version | head -n1 | cut -d' ' -f4 | sed -e 's/\.//'`
if [ "$RES" -lt "268" ]; then
    IGNORE_OLD_TOOLCHAIN="${1:-no}"
    if [[ "$IGNORE_OLD_TOOLCHAIN" != "geezer-os" ]] && [[ "$IGNORE_OLD_TOOLCHAIN" != "old-fart" ]] ; then

        echo
        echo "###"
        echo "### WARNING:"
        echo "### WARNING: Your autotools version is too old."
        echo "### WARNING:"
        echo "###"
        echo "### More information and workaround is available here:"
        echo "###     https://github.com/a2o/snoopy/blob/master/doc/HACKING.md#older-oses"
        echo "###"
        echo "### The workaround is now performed by ./bootstrap.sh automatically."
        echo "### This warning can be suppressed by using the following command:"
        echo "###"
        echo "###     ./bootstrap.sh geezer-os"
        echo "###"
        echo

    else

        echo
        echo "### INFO: Ignoring too-old autotools suite."
        echo
    fi

    echo    "BOOTSTRAP: Adjusting configure.ac for old OS:"
    MYDIR=`dirname $0`
    SNOOPY_VERSION_LITERAL=`$MYDIR/build/get-version.sh | sed -e 's/$/-geezer-os/'`
    SNOOPY_VERSION_CONFIGURE_AC=`echo "$SNOOPY_VERSION_LITERAL" | sed -e 's/^/    [/' | sed -e 's/$/],/'`
    echo -n "BOOTSTRAP:   - version from script to literal string '$SNOOPY_VERSION_LITERAL'... "
    sed -i "s/^    m4_esyscmd_s.*/$SNOOPY_VERSION_CONFIGURE_AC/" configure.ac
    echo "done."
    echo "BOOTSTRAP:   All done."
    echo "BOOTSTRAP: IMPORTANT NOTICE: Do not commit these changes!"
    echo
fi



### Check if submodules are checked out, and do it if not
#
if [ ! -e lib/iniparser/.git ]; then
    echo "###"
    echo "### Cloning git submodules:"
    echo "###"
    echo
    git submodule init
    git submodule update
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
if [ ! -d lib/iniparser/build/m4 ]; then
    mkdir -p lib/iniparser/build/m4
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
echo "### AutoTools run was completed sucessfully."
echo "### You should run ./configure now."
echo "###"
echo
