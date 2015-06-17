#!/bin/bash



### Settings
#
PUBLIC_GIT_REMOTE_NAME="github-a2o"



### Check working directory
if [ ! -d "doc/releasing" ]; then
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



### Check if first line of tagged commit contains "Release X.Y.Z"
RES=`git log -1 --pretty="%B" $RELEASE_TAG | head -n1 | grep "^Release $RELEASE_VERSION\$" -c`
if [ "$RES" -ne "1" ]; then
    echo "ERROR: Release commit does not contain this message as first line: 'Release $RELEASE_VERSION'"
    exit 2
fi



### Check if git remote with required name exists
RES=`git remote | grep "^$PUBLIC_GIT_REMOTE_NAME\$" -c`
if [ "$RES" -ne "1" ]; then
    echo "ERROR: Required git remote not found: '$PUBLIC_GIT_REMOTE_NAME'"
    exit 2
fi



### Check for uncommited changes in the current repo
#
# Uncommited changes are NOT allowed
#
RES=`git describe --always --dirty | grep -- '-dirty$' | grep -c .`
if [ "$RES" -ne "0" ]; then
    echo "ERROR: There are uncommited changes in the repository. Commit/stash them and try again."
    echo
    git status -s
    echo
    exit 2
fi



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

### Determine if this version is stable or development release
if [[ $RELEASE_VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    RELEASE_IS_STABLE="1"
else
    RELEASE_IS_STABLE="0"
fi






### Check that release dirs and files do not exists
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



### Remember current git branch
GIT_BRANCH_CUR=`git rev-parse --abbrev-ref HEAD`
echo "RELEASING: Currently on git branch '$GIT_BRANCH_CUR', will return to it at the end..."



### Checkout
echo "RELEASING: Checking out git tag '$RELEASE_TAG'..."
git checkout $RELEASE_TAG



### Check for release in ChangeLog
RES=`cat ChangeLog | grep "$RELEASE_VERSION\$" | cat`
if [ "x$RES" == "x" ]; then
    echo "ERROR: Release version $RELEASE_VERSION not found in ChangeLog file."
    exit 20
fi

### Check for release in README.md
RES=`cat README.md | grep snoopy-$RELEASE_VERSION\.t | cat`
if [ "x$RES" == "x" ]; then
    echo "ERROR: Release version $RELEASE_VERSION not found in README.md file."
    exit 20
fi



### Create release package
./bootstrap.sh &&
./configure --enable-everything &&
make clean &&
make distcheck &&



### Checkout back master branch
echo "RELEASING: Restoring original git branch '$GIT_BRANCH_CUR'..."
git checkout $GIT_BRANCH_CUR &&



### Create package
echo "RELEASING: Creating release tarball hashes..."
md5sum  $FILENAME_RELEASE > $FILE_RELEASE_MD5  &&
sha1sum $FILENAME_RELEASE > $FILE_RELEASE_SHA1 &&
wc -c   $FILENAME_RELEASE > $FILE_RELEASE_SIZE &&

# These two are used by snoopy-install.sh script
echo "$RELEASE_VERSION"  > $FILE_LATEST_VERSION &&
echo "$FILENAME_RELEASE" > $FILE_LATEST_PACKAGE_FILENAME &&



### Upload release files
echo &&
echo "RELEASING: Uploading release package files to http://source.a2o.si/download/snoopy/..." &&
echo &&
scp \
    $FILE_RELEASE \
    $FILE_RELEASE_MD5 \
    $FILE_RELEASE_SHA1 \
    $FILE_RELEASE_SIZE \
    $PUBLIC_DL_SSH_HOST:$PUBLIC_DL_SSH_PATH \
    &&
echo &&



### If stable release, update latest stable package version metadata
if [ "$RELEASE_IS_STABLE" == "1" ]; then
    echo "RELEASING: This is a stable release." &&
    echo "RELEASING: Updating files that indicate latest package version..." &&
    echo &&
    scp \
        $FILE_LATEST_VERSION \
        $FILE_LATEST_PACKAGE_FILENAME \
        $PUBLIC_DL_SSH_HOST:$PUBLIC_DL_SSH_PATH \
        &&

    ssh $PUBLIC_DL_SSH_HOST \
        ln -sf $FILENAME_RELEASE $PUBLIC_DL_SSH_PATH/snoopy-latest.tar.gz
else
    echo "RELEASING: This is NOT a stable release." &&
    echo "RELEASING: Skipped updating files that indicate latest package version."
    echo
fi &&

echo &&
echo "COMPLETE: $RELEASE_TAG has been released." &&
echo "DL URI:   $PUBLIC_DL_URI_PREFIX/$FILENAME_RELEASE" &&
echo



### Push code and tags to GitHub
#
# Doing this once with --tags argument only does not push actual code, only tags
#
echo &&
echo "RELEASING: Pushing code and tags to GitHub..." &&
echo &&
git push $PUBLIC_GIT_REMOTE_NAME &&
git push $PUBLIC_GIT_REMOTE_NAME --tags &&



### Remove created package files
echo &&
echo "CLEANUP: Running 'make clean'... (output suppressed)" &&
echo &&
make clean > /dev/null &&



### All done.
true
