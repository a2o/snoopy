#!/bin/bash



### Shell configuration and error handler
#
set -e
set -u



### Fatal error handler
#
_fatalError() {
    ERR_FILE="$0"
    ERR_MSG="$1"
    ERR_LINE="${2:--}"
    echo "[$ERR_FILE:$ERR_LINE] ERROR: $ERR_MSG" 1>&2
    exit 1
}



### Message output handler
#
_echo() {
    echo "[$0] $1"
}



### Release tag verifier
#
_isReleaseTagFormatValid() {
    RELEASE_TAG_TO_VERIFY="$1"
    if [[ $RELEASE_TAG_TO_VERIFY =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        true
    else
        false
    fi
}



### Release version verifier
#
_isReleaseVersionFormatValid() {
    RELEASE_VERSION_TO_VERIFY="$1"
    if [[ $RELEASE_VERSION_TO_VERIFY =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        true
    else
        false
    fi
}



### Check the runtime environment
#
if [ "${BOOTSTRAP_GIT_REPO_REQUIRED:-true}" != "false" ]; then
    if [ ! -d .git ]; then
        _fatalError "This script must be run from the root of the git repository"
    fi
fi
