#!/bin/bash



### Description
#
# Return the git tag representing the state of the working directory.
#
# Expected output variants:
#   - snoopy-2.3.1
#   - snoopy-2.3.1-dirty/branch-name
#   - snoopy-2.3.1-146-COMMITID/branch-name
#   - snoopy-2.3.1-146-COMMITID-dirty/branch-name
#
# The "-dirty" suffix is added whenever there are uncommitted changes in the
# working directory.
#
# The "-xxx-COMMITID" part is added whenever the currently checked out commit
# does not have an associated git tag. "xxx" represents the number of commits
# since the latest tag. "COMMITID" is a hash representing the current commit.



### Shell configuration and error handler
#
set -e
set -u

_fatalError() {
    MSG="$1"
    echo "ERROR($0): $MSG" 1>&2
    exit 1
}



### Check the runtime environment
#
if [ ! -d .git ]; then
    _fatalError "This script must be run from the root of the git repository"
fi



### Generate the data
#
SNOOPY_RELEASE_VERSION=`./dev-tools/libexec/get-release-version.sh ${@:-}`
SNOOPY_RELEASE_TAG="snoopy-$SNOOPY_RELEASE_VERSION"



### Add branch info for SonarCloud tags, if applicable
#
SONARCLOUD_TAG="$SNOOPY_RELEASE_TAG"
if ! [[ $SONARCLOUD_TAG =~ ^snoopy-[0-9]+.[0-9]+.[0-9]+(rc[0-9]+)?$ ]]; then
    CURRENT_BRANCH=`git branch --show-current`
    SONARCLOUD_TAG="$SONARCLOUD_TAG/$CURRENT_BRANCH"
fi
echo "$SONARCLOUD_TAG"
