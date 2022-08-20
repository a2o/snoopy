#!/bin/bash



### Shell configuration and error handler
#
set -e
set -u
set -o pipefail



### Fatal error handler
#
_fatalError() {
    ERR_FILE="$0"
    ERR_MSG="$1"
    ERR_LINE="${2:--}"
    echo "[$ERR_FILE:$ERR_LINE] ERROR: $ERR_MSG" 1>&2
    exit 1
}



### Message output handlers
#
_echo() {
    echo "[$0] $1"
}
_debug() {
    echo "[$0] [DEBUG] $1"
}



### Public release version/tag verifier
#
# This check (in contrast to the one below) is more strict, and controls what
# format of public release versioning do we actually allow in the current
# releasing process implementation. For now, only these two options:
# - X.Y.Z
# - X.Y.ZrcN
#
_isPublicReleaseTagFormatValid() {
    RELEASE_TAG_TO_VERIFY="$1"
    if [[ $RELEASE_TAG_TO_VERIFY =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+(rc[0-9]+)?$ ]]; then
        true
    else
        false
    fi
}
_isPublicReleaseVersionFormatValid() {
    RELEASE_VERSION_TO_VERIFY="$1"
    if [[ $RELEASE_VERSION_TO_VERIFY =~ ^[0-9]+\.[0-9]+\.[0-9]+(rc[0-9]+)?$ ]]; then
        true
    else
        false
    fi
}



### Determine if release is considered stable or not
#
# RC and git-based releases are not considered stable.
#
_doesReleaseTagDenoteStableRelease() {
    RELEASE_TAG_TO_VERIFY="$1"
    if [[ $RELEASE_TAG_TO_VERIFY =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        true
    else
        false
    fi
}
_doesReleaseVersionDenoteStableRelease() {
    RELEASE_VERSION_TO_VERIFY="$1"
    if [[ $RELEASE_VERSION_TO_VERIFY =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        true
    else
        false
    fi
}



### Determine if release is considered public or not
#
# Stable and RC releases are considered public. Implications (as of this writing)
# are that such releases are subject to more consistency scrutiny.
#
# Git-based preview releases (with -X-gXXXXXXX version suffix) are not considered
# a public repease, even if their packages land in the `testing` channel.
#
_doesReleaseTagDenotePublicRelease() {
    RELEASE_TAG_TO_VERIFY="$1"
    if [[ $RELEASE_TAG_TO_VERIFY =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+(-?rc\.?[0-9]+)?$ ]]; then
        true
    else
        false
    fi
}
_doesReleaseVersionDenotePublicRelease() {
    RELEASE_VERSION_TO_VERIFY="$1"
    if [[ $RELEASE_VERSION_TO_VERIFY =~ ^[0-9]+\.[0-9]+\.[0-9]+(-?rc\.?[0-9]+)?$ ]]; then
        true
    else
        false
    fi
}



### Determine packaging channel from tag/version
#
# Usage:
#       PACKAGING_CHANNEL=""
#       _getPackagingChannelFromVersion "$MY_SNOOPY_VERSION"
#       if [ "$PACKAGING_CHANNEL" == "stable" ]; then
#           ...
#       if
#
_getPackagingChannelFromTag() {
    RELEASE_TAG_TO_VERIFY="$1"
    PACKAGING_CHANNEL=""
    if [[ $RELEASE_TAG_TO_VERIFY =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        PACKAGING_CHANNEL="stable"
    else
        PACKAGING_CHANNEL="testing"
    fi
}
_getPackagingChannelFromVersion() {
    RELEASE_VERSION_TO_VERIFY="$1"
    PACKAGING_CHANNEL=""
    if [[ $RELEASE_VERSION_TO_VERIFY =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
        PACKAGING_CHANNEL="stable"
    else
        PACKAGING_CHANNEL="testing"
    fi
}



_detectOperatingSystem()
{
    # NOTICE: Keep this code in sync in the following files:
    #   - dev-tools/_bootstrap.sh
    #   - dev-tools/install-dev-software.sh
    #   - dev-tools/install-packaging-software.sh
    #   - install/install-snoopy.sh
    #
    # Expects:
    #   - Global variable OS_ID set to ""
    #   - Global variable OS_VERSION set to ""
    #   - Global variable OS_VERSION_CODENAME set to ""
    #
    # Sets:
    #   - Global variable OS_ID
    #   - Global variable OS_VERSION (potentially not applicable on rolling releases)
    #   - Global variable OS_VERSION_CODENAME (if applicable)
    #
    # How to use this method in a script:
    #   OS_ID=""
    #   OS_VERSION=""
    #   OS_VERSION_CODENAME=""
    #   _detectOperatingSystem
    #   echo "id:$OS_ID"
    #   echo "version=$OS_VERSION"
    #   echo "version_codename=$OS_VERSION_CODENAME"
    #
    # Returns:
    #   - (nothing)
    OS_ID=""
    OS_VERSION=""
    OS_VERSION_CODENAME=""

    . /etc/os-release

    OS_ID="$ID"
    OS_VERSION="${VERSION_ID:-}"
    OS_VERSION_CODENAME="${VERSION_CODENAME:-}"

    if [ "$OS_ID" == "" ]; then
        _debug "/etc/os-release content:"
        cat /etc/os-release
        _fatalError "Unable to detect your OS via /etc/os-release."
    fi

    # Debian Sid quirk
    if [[ "$OS_ID" == "debian" ]] && [[ "$OS_VERSION_CODENAME" == "" ]]; then
        OS_VERSION_CODENAME=`apt-cache policy | grep http://deb.debian.org/debian -A1 | tail -n1 | grep -Eo 'n=[a-z]+' | sed -e 's/^n=//'`
    fi

    # Almalinux contains minor version in VERSION_ID, let's remove it
    if [[ "$OS_ID" == "almalinux" ]] && [[ $OS_VERSION =~ \. ]]; then
        OS_VERSION=`echo "$OS_VERSION" | cut -d. -f1`
    fi

    # Arch quirk
    if [[ "$OS_ID" == "arch" ]] && [[ "$OS_VERSION" == "TEMPLATE_VERSION_ID" ]]; then
        OS_VERSION=""
        OS_VERSION_CODENAME=""
    fi
}



_isGitStatusIgnoredClean() {
    RES=`git status --ignored --short | grep -c '^!!'` || true
    if [ "$RES" -le "2" ]; then
        return 0
    else
        return 1
    fi
}



### Check the runtime environment
#
if [ "${BOOTSTRAP_GIT_REPO_REQUIRED:-true}" != "false" ]; then
    if [ ! -d .git ]; then
        _fatalError "This script must be run from the root of the git repository"
    fi
fi
