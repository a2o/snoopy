#!/bin/bash



### Check working directory
if [ ! -d "dev-scripts" ]; then
    echo "ERROR: You have to run this script in the root of the git repository"
    exit 1
fi



### Check release tag
RELEASE_TAG="$1"
if [ "x$RELEASE_TAG" == "x" ]; then
    echo "ERROR: No release tag specified. Please use:   $0 snoopy-X.Y.Z"
    exit 1
fi



### Check if release tag is properly formatted
if [[ ! "$RELEASE_TAG" =~ ^snoopy- ]]; then
    echo "ERROR: Release tag is not properly formatted - snoopy-x.y.z format is required"
    exit 1
fi
RELEASE_VERSION=`echo "$RELEASE_TAG" | sed -e 's/snoopy-//'`



### Check if release tag exists
RES=`git tag | grep "^$RELEASE_TAG\$"`
if [ "$RES" != "$RELEASE_TAG" ]; then
    echo "ERROR: Release tag does not exist, please create it with:   git tag snoopy-X.Y.Z"
    exit 2
fi



### Check for uncommited changes in the current repo
#RES=`git diff`
#if [ "x$RES" != "x" ]; then
#	echo "ERROR: Uncommited changes in current working copy. Please commit/stash and try again."
#	exit 3
#fi



### Paths and filenames
DIR_REPO=`pwd`
FILENAME_RELEASE="snoopy-$RELEASE_VERSION.tar.gz"
FILENAME_RELEASE_MD5="snoopy-$RELEASE_VERSION.tar.gz.md5"
FILENAME_RELEASE_SHA1="snoopy-$RELEASE_VERSION.tar.gz.sha1"
FILE_RELEASE="$DIR_REPO/$FILENAME_RELEASE"
FILE_RELEASE_MD5="$DIR_REPO/$FILENAME_RELEASE_MD5"
FILE_RELEASE_SHA1="$DIR_REPO/$FILENAME_RELEASE_SHA1"



### Check if release dirs and files do not exists
if [ -e $FILE_RELEASE ]; then
    echo "ERROR: Release file already exists: $FILE_RELEASE"
    exit 10
fi
if [ -e $FILE_RELEASE_MD5 ]; then
    echo "ERROR: Release MD5 file already exists: $FILE_RELEASE_MD5"
    exit 10
fi
if [ -e $FILE_RELEASE_SHA1 ]; then
    echo "ERROR: Release SHA1 file already exists: $FILE_RELEASE_SHA1"
    exit 10
fi



### Checkout
git checkout $RELEASE_TAG



### Check for release tags
RES=`cat configure.ac | fgrep $RELEASE_VERSION | cat`
if [ "x$RES" == "x" ]; then
    echo "ERROR: Release tag $RELEASE_TAG not found in configure.ac file."
    exit 20
fi



### Create release package
./autogen.sh &&
./configure &&
make clean &&
make dist &&



### Checkout back master branch
git checkout master &&



### Create package
md5sum  $FILE_RELEASE > $FILE_RELEASE_MD5  &&
sha1sum $FILE_RELEASE > $FILE_RELEASE_SHA1 &&



### Upload releases
echo &&
echo "RELEASING: Pushing code and tags to GitHub..." &&
echo &&
git push github &&
git push github --tags &&

echo &&
echo "RELEASING: Uploading release files to http://source.a2o.si/download/snoopy/..." &&
echo &&
scp $FILE_RELEASE $FILE_RELEASE_MD5 $FILE_RELEASE_SHA1 source.a2o.si:/var/www/source.a2o.si/public/download/snoopy/ &&

echo &&
echo "COMPLETE: $RELEASE_TAG has been released." &&
echo &&



### All done.
true
