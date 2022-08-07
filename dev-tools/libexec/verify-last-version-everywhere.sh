#!/bin/bash



### How does this script operate?
#
# Script's expectations:
# - TODO
#
# Script's arguments:
# - Release version to verify
#
# Steps that this script performs:
# - Takes the release version as an argument
# - Inspects ChangeLog
# - Makes sure that the top-most listed release is the one we're verifying for



### Shell configuration and script bootstrap
#
set -e
set -u
. `dirname $0`/../_bootstrap.sh



### Get the release version from arguments
#
RELEASE_VERSION="${1:-}"
if [ "$RELEASE_VERSION" == "" ]; then
    _fatalError "Release version argument not provided. Usage: $0 x.y.z"
fi



### Verify release version syntax
#
if ! _isReleaseVersionFormatValid "$RELEASE_VERSION"; then
    _fatalError "Invalid release version syntax: $RELEASE_VERSION" $LINENO
fi



### Check everywhere for last version consistency
#
./dev-tools/libexec/verify-last-version-in-readme.sh             "$RELEASE_VERSION"
./dev-tools/libexec/verify-last-version-in-changelog.sh          "$RELEASE_VERSION"
#./dev-tools/libexec/verify-last-version-in-packaging-debian.sh   "$RELEASE_VERSION"



### All good
#
true
