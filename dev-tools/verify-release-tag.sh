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
    echo "[$0] $1"
}



### Check the runtime environment
#
if [ ! -d .git ]; then
    _fatalError "This script must be run from the root of the git repository"
fi



### Get the release version
#
RELEASE_TAG="${1:-}"
if [ "$RELEASE_TAG" == "" ]; then
    _fatalError "Release tag argument missing. Usage: $0 snoopy-X.Y.Z"
fi
if [[ ! $RELEASE_TAG =~ ^snoopy- ]]; then
    _fatalError "Release tag argument invalid: '$RELEASE_TAG'. Valid format: 'snoopy-X.Y.Z'"
fi
RELEASE_VERSION=`echo "$RELEASE_TAG" | sed -e 's/snoopy-//'`



### Check if release tag actually exists
#
if ! git tag | grep "^$RELEASE_TAG\$" > /dev/null ; then
    _fatalError "Release tag not found: $RELEASE_TAG"
fi



### Check if first line of the tagged commit contains "Release X.Y.Z"
#
RES=`git log -1 --pretty="%B" $RELEASE_TAG | head -n1 | grep "^Release $RELEASE_VERSION\$" -c | cat`
if [ "$RES" -ne "1" ]; then
    _fatalError "Release commit message under tag $RELEASE_TAG does not contain the following first line: 'Release $RELEASE_VERSION'"
fi



### Check if ChangeLog has the same release
#
RELEASE_VERSION_CHANGELOG=`./dev-tools/libexec/get-release-version.sh changelog`
if [ "$RELEASE_VERSION" != "$RELEASE_VERSION_CHANGELOG" ]; then
    _fatalError "Release version mismatch (checking: $RELEASE_VERSION; latest in the ChangeLog file: $RELEASE_VERSION_CHANGELOG)"
fi



### Check if README.md - NEWS has the same release
#
RES=`cat README.md | grep -E '^## News' -A10 | grep -E "Snoopy $RELEASE_VERSION released!" -c | cat`
if [ "$RES" != "1" ]; then
    _fatalError "News section in README.md does not contain a reference to $RELEASE_VERSION version."
fi



### All done.
_echo "Release tag checks passed for: $RELEASE_TAG"
true
