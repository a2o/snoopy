#!/bin/bash



### Check working directory
if [ ! -d "dev-scripts" ]; then
	echo "ERROR: You have to run this script in the root of repository"
	exit 1
fi



### Check release tag
RELEASE_TAG="$1"
if [ "x$RELEASE_TAG" == "x" ]; then
	echo "ERROR: No release tag given. Use $0 X.Y.Z"
	exit 1
fi



### Check if release already exists
RES=`git tag | grep "^$RELEASE_TAG\$"`
if [ "$RES" != "$RELEASE_TAG" ]; then
	echo "ERROR: Release tag does not exist, please create it with 'git tag X.Y.Z"
	exit 2
fi



### Create copy of trunk
svn cp trunk tags/release-$RELEASE_TAG &&
svn ci -m "Commiting release $RELEASE_TAG to svn" &&



# Create temporary directory and copy release there
cp -pR tags/release-$RELEASE_TAG snoopy-$RELEASE_TAG &&
cd snoopy-$RELEASE_TAG &&
autoheader &&
autoconf &&
cd .. &&
tar -c -z -f snoopy-$RELEASE_TAG.tar.gz snoopy-$RELEASE_TAG &&
rm -rf snoopy-$RELEASE_TAG
