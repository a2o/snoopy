#!/bin/bash



### Check working directory
if [ ! -d "dev-scripts" ]; then
	echo "ERROR: You have to run this script in the root of the git repository"
	exit 1
fi



### Check release tag
RELEASE_TAG="$1"
if [ "x$RELEASE_TAG" == "x" ]; then
	echo "ERROR: No release tag specified. Please use:   $0 X.Y.Z"
	exit 1
fi



### Check if release tag is properly formatted
if [[ ! "$RELEASE_TAG" =~ ^snoopy- ]]; then
    echo "ERROR: Release tag is not properly formatted - snoopy-x.y.z format is required"
    exit 1
fi



### Check if release tag exists
RES=`git tag | grep "^$RELEASE_TAG\$"`
if [ "$RES" != "$RELEASE_TAG" ]; then
	echo "ERROR: Release tag does not exist, please create it with:   git tag X.Y.Z"
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
DIR_REPO_PARENT=`dirname $DIR_REPO`
DIRNAME_RELEASE="snoopy-$RELEASE_TAG"
FILENAME_RELEASE="snoopy-$RELEASE_TAG.tar.gz"
FILENAME_RELEASE_MD5="snoopy-$RELEASE_TAG.tar.gz.md5"
FILENAME_RELEASE_SHA1="snoopy-$RELEASE_TAG.tar.gz.sha1"
DIR_RELEASE="$DIR_REPO_PARENT/$DIRNAME_RELEASE"
FILE_RELEASE="$DIR_REPO_PARENT/$DIRNAME_RELEASE.tar.gz"
FILE_RELEASE_MD5="$DIR_REPO_PARENT/$DIRNAME_RELEASE.tar.gz.md5"
FILE_RELEASE_SHA1="$DIR_REPO_PARENT/$DIRNAME_RELEASE.tar.gz.sha1"



### Check if release dirs and files do not exists
if [ -e $DIR_RELEASE ]; then
	echo "ERROR: Release directory already exists: $DIR_RELEASE"
	exit 10
fi
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
RES=`cat configure.ac | fgrep $RELEASE_TAG | cat`
if [ "x$RES" == "x" ]; then
	echo "ERROR: Release tag $RELEASE_TAG not found in configure.ac file."
	exit 20
fi



### Create release directory
mkdir -p $DIR_RELEASE &&
cp -pR $DIR_REPO/* $DIR_RELEASE &&
cd $DIR_RELEASE &&
rm -rf dev-scripts &&
autoheader &&
autoconf



### Compare configure.ac and configure.ac.generated
#diff configure.ac configure.ac.generated
#if [ "$?" != "0" ]; then
#	echo "ERROR: configure.ac and configure.ac.generated differ."
#	exit 40
#fi



### Create package
cd .. &&
tar -c -z -f $FILENAME_RELEASE $DIRNAME_RELEASE &&
md5sum  $FILENAME_RELEASE > $FILENAME_RELEASE_MD5  &&
sha1sum $FILENAME_RELEASE > $FILENAME_RELEASE_SHA1 &&
rm -rf $DIRNAME_RELEASE &&



### Check out master branch again
cd $DIR_REPO &&
git checkout master &&



### Upload releases
echo &&
echo "RELEASING: Pushing code and tags to GitHub..." &&
echo &&
git push github &&
git push github --tags &&

echo &&
echo "RELEASING: Pushing code to http://source.a2o.si/download/snoopy/..." &&
echo &&
scp $FILE_RELEASE $FILE_RELEASE_MD5 $FILE_RELEASE_SHA1 a2o.source:public/download/snoopy/ &&

echo
echo "COMPLETE: $RELEASE_TAG has been published."
echo
