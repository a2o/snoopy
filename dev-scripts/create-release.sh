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
FILENAME_RELEASE_SIZE="snoopy-$RELEASE_VERSION.tar.gz.size"
FILENAME_LATEST_VERSION="snoopy-latest-version.txt"
FILENAME_LATEST_PACKAGE_FILENAME="snoopy-latest-package-filename.txt"
FILE_RELEASE="$DIR_REPO/$FILENAME_RELEASE"
FILE_RELEASE_MD5="$DIR_REPO/$FILENAME_RELEASE_MD5"
FILE_RELEASE_SHA1="$DIR_REPO/$FILENAME_RELEASE_SHA1"
FILE_RELEASE_SIZE="$DIR_REPO/$FILENAME_RELEASE_SIZE"
FILE_LATEST_VERSION="$DIR_REPO/$FILENAME_LATEST_VERSION"
FILE_LATEST_PACKAGE_FILENAME="$DIR_REPO/$FILENAME_LATEST_PACKAGE_FILENAME"
PUBLIC_DL_SSH_HOST="source.a2o.si"
PUBLIC_DL_SSH_PATH="/var/www/source.a2o.si/public/download/snoopy"
PUBLIC_DL_URI_PREFIX="http://source.a2o.si/download/snoopy"



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
if [ -e $FILE_RELEASE_SIZE ]; then
    echo "ERROR: Release SIZE file already exists: $FILE_RELEASE_SIZE"
    exit 10
fi



### Check if we can connect to SSH host
echo "SSH TEST: Checking SSH connectivity for final release tarball upload."
echo "SSH TEST: If you are promted for account password (not private key password), it does not work."
echo "SSH TEST: If that happens, just hit CTRL+C and fix the problem before trying publish a release."
ssh $PUBLIC_DL_SSH_HOST true
if [ "$?" != "0" ]; then
    echo "ERROR: Failed to test-connect to the $PUBLIC_DL_SSH_HOST via SSH. Aborting."
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
md5sum  $FILENAME_RELEASE > $FILE_RELEASE_MD5  &&
sha1sum $FILENAME_RELEASE > $FILE_RELEASE_SHA1 &&
wc -c   $FILENAME_RELEASE > $FILE_RELEASE_SIZE &&

# These two are used by snoopy-install.sh script
echo "$RELEASE_VERSION"  > $FILE_LATEST_VERSION &&
echo "$FILENAME_RELEASE" > $FILE_LATEST_PACKAGE_FILENAME &&



### Upload releases
echo &&
echo "RELEASING: Pushing code and tags to GitHub..." &&
echo &&
git push github &&
git push github --tags &&

echo &&
echo "RELEASING: Uploading release files to http://source.a2o.si/download/snoopy/..." &&
echo &&
scp \
    $FILE_RELEASE \
    $FILE_RELEASE_MD5 \
    $FILE_RELEASE_SHA1 \
    $FILE_RELEASE_SIZE \
    $FILE_LATEST_VERSION \
    $FILE_LATEST_PACKAGE_FILENAME \
    $PUBLIC_DL_SSH_HOST:$PUBLIC_DL_SSH_PATH \
    &&

# Symlink latest files
echo &&
echo "RELEASING: Creating symlink snoopy-latest.tar.gz..." &&
echo &&
ssh $PUBLIC_DL_SSH_HOST ln -sf $FILENAME_RELEASE $PUBLIC_DL_SSH_PATH/snoopy-latest.tar.gz &&

echo &&
echo "COMPLETE: $RELEASE_TAG has been released." &&
echo "DL URI:   $PUBLIC_DL_URI_PREFIX/$FILENAME_RELEASE" &&
echo &&



### All done.
true
