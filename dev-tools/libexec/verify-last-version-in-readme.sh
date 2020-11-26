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



### Check the `News` section of README.md
#
if ! cat README.md | grep -E '^## News$' -A10 | grep -E '^[|] [0-9]{4}-[0-9]{2}-[0-9]{2}' | grep -E "Snoopy $RELEASE_VERSION released[!.]" > /dev/null; then
    _fatalError "Version $RELEASE_VERSION not mentioned in the 'News' section of the README.md" $LINENO
fi



### Check the `Latest version` section of README.md
#
if ! cat README.md | grep -E '^## Latest version$' -A10 | grep -E "https://github.com/a2o/snoopy/releases/download/snoopy-$RELEASE_VERSION/snoopy-$RELEASE_VERSION.tar.gz" > /dev/null; then
    _fatalError "Version $RELEASE_VERSION not mentioned in the 'Latest version' section of the README.md" $LINENO
fi



### All good
#
true
