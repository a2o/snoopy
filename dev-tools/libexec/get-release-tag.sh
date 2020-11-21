#!/bin/bash



### Description
#
# Return the git tag representing the state of the working directory.
#
# Expected output variants:
#   - snoopy-2.3.1
#   - snoopy-2.3.1-dirty
#   - snoopy-2.3.1-146-COMMITID
#   - snoopy-2.3.1-146-COMMITID-dirty
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
echo "$SNOOPY_RELEASE_TAG"
