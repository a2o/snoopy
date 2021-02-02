#!/bin/bash



### Shell configuration and error handler
#
set -e
set -u

_fatalError() {
    echo "ERROR($0): $1" 1>&2
    exit 1
}
_echo() {
    echo "[$0]: $1"
}



### Check the runtime environment
#
if [ ! -d .git ]; then
    _fatalError "This script must be run from the root of the git repository"
fi



### Get the release tag
#
RELEASE_TAG=`./dev-tools/libexec/get-release-tag.sh`
if [ "$RELEASE_TAG" == "" ]; then
    _fatalError "Unable to determine release tag, got: '$RELEASE_TAG'"
fi



### Check if release tag is properly formatted
#
if [[ ! "$RELEASE_TAG" =~ ^snoopy- ]]; then
    _fatalError "Release tag is not properly formatted - snoopy-* format is required"
fi
RELEASE_VERSION=`echo "$RELEASE_TAG" | sed -e 's/snoopy-//'`

_echo "Determined release tag:     $RELEASE_TAG"
_echo "Determined release version: $RELEASE_VERSION"



### If this is a production build, do additional checking
#
if [[ $RELEASE_TAG =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+(-rc[0-9]+)?$ ]]; then
    _echo "This is a stable production build, running additional consistency checks..."
    ./dev-tools/verify-release-tag.sh "$RELEASE_TAG"
    RELEASE_IS_STABLE="1"
else
    _echo "This is a non-stable/non-production build, additional consistency checks will be skipped."
fi



### Does the target release package already exist?
#
RELEASE_PACKAGE_FILE="snoopy-$RELEASE_VERSION.tar.gz"
if [ -e $RELEASE_PACKAGE_FILE ]; then
    _fatalError "Release package file already exists: $RELEASE_PACKAGE_FILE"
fi



### Create the release package
#
./bootstrap.sh
./configure --enable-everything
make clean
make distcheck



### All done.
_echo "Build complete."
_echo "Result: $RELEASE_PACKAGE_FILE"
