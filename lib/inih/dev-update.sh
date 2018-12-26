#!/bin/bash



### Configure shell
#
set -e
set -u
GITORIGINURL="https://github.com/benhoyt/inih.git"
GITREF="master"
TMPGITDIR="./_tmp-inih-git"
DESTDIR="."
DESTDIRSRC="./src"



### Parse arguments
#
if [ "${1:-}" != "" ]; then
    GITREF="$1"
fi
echo "Using gitref: $GITREF"



### Clone the repo
#
rm -rf $TMPGITDIR
git clone   $GITORIGINURL   $TMPGITDIR
(cd $TMPGITDIR && git checkout $GITREF)



### Copy the files
#
cp $TMPGITDIR/ini.c $DESTDIRSRC
cp $TMPGITDIR/ini.h $DESTDIRSRC



### Apply patches
#
patch -p0 < ./patches/0001-strip-value-quotes.diff



### Get the version information
#
INIH_VERSION=`cd $TMPGITDIR && git describe --all --long`
cat << EOF > $DESTDIR/SOURCE.txt
git-origin-url = '$GITORIGINURL'
git-origin-ref = '$INIH_VERSION'
patches-dir = 'patches/'
EOF



### Remove the tmp git dir
#
rm -rf $TMPGITDIR



### All good.
#
echo "Any changes?"
git status $DESTDIR

echo
echo "Update succeeded."
