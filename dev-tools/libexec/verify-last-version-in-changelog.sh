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
    _fatalError "Release version argument not provided. Usage: $0 x.y.z" $LINENO
fi



### Verify release version syntax
#
if ! _isReleaseVersionFormatValid "$RELEASE_VERSION"; then
    _fatalError "Invalid release version syntax: $RELEASE_VERSION" $LINENO
fi



### Get latest release version from ChangeLog and verify it
#
LAST_VERSION_IN_CHANGELOG=`cat ChangeLog | grep -E '^[0-9]{4}-[0-9]{2}-[0-9]{2} - Version [0-9]+\.[0-9]+\.[0-9]+$' | head -n1 | awk '{print $4}'`
if [ "$LAST_VERSION_IN_CHANGELOG" != "$RELEASE_VERSION" ]; then
    _fatalError "Last version listed in ChangeLog is $LAST_VERSION_IN_CHANGELOG, not $RELEASE_VERSION" $LINENO
fi



### All good
#
#_echo "Last version listed in ChangeLog: $LAST_VERSION_IN_CHANGELOG"
true
