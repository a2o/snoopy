#!/bin/bash



### Shell configuration and script bootstrap
#
set -e
set -u
set -o pipefail
. `dirname $0`/_bootstrap.sh



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



### Verify version consistency for public releases
#
RELEASE_IS_PUBLIC="false"
if _doesReleaseTagDenotePublicRelease "$RELEASE_TAG" ; then
    _echo "This is a stable/rc build, running version consistency checks..."
    ./dev-tools/libexec/verify-last-version-everywhere.sh "$RELEASE_VERSION"
    RELEASE_IS_PUBLIC="true"
else
    _echo "This is NOT a stable/rc build, thus skipping version consistency checks."
fi



### Does the target release package already exist?
#
RELEASE_PACKAGE_FILE="snoopy-$RELEASE_VERSION.tar.gz"
if [ -e $RELEASE_PACKAGE_FILE ]; then
    _fatalError "Release package file already exists: $RELEASE_PACKAGE_FILE"
fi



### Clean up the git repo first
#
./dev-tools/clean-git-repository.sh



### Create the release package
#
./bootstrap.sh
./configure --enable-everything
make clean
make distcheck



### Report success
#
_echo ""
_echo "SUCCESS: Build complete."
_echo "SUCCESS: Result: $RELEASE_PACKAGE_FILE"
_echo ""



### Suggest next step(s)
#
if [ "$RELEASE_IS_PUBLIC" == "true" ]; then
    _echo "
Next step:
==========

5. Publish the release:

    ./dev-tools/publish-release.sh

"
else
    _echo "
Next step:
==========

This is not a public release, thus not showing the publishing next step(s).

"
fi
