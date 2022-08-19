#!/bin/bash



### Description
#
# Return the version representing the state of the working directory.
#
# Expected output variants:
#   - 2.3.1
#   - 2.3.1-dirty
#   - 2.3.1-146-COMMITID
#   - 2.3.1-146-COMMITID-dirty
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
if [ ! -d dev-tools ]; then
    _fatalError "This script must be run from the root of either git repository or uncompressed distribution package directory"
fi



### Define the help method
#
_showHelp()
{
    cat <<EOF
Purpose:

    Return the version of Snoopy code in the source tree

Supported CLI arguments:

    -i          Ignore a dirty working directory - do not add '-dirty' suffix to the version.
                Only applicable in 'git' mode (and when 'autoreconf' mode uses git too).
    -m MODE     Specify the mode of operation ('git', 'changelog', or 'autoreconf' [default]).
                Mode 'autoreconf' uses 'git' mode if available, and falls back on 'changelog'
                mode (i.e. when used within a distribution tarball).

    -h/--help   Show this help.

Supported environment variables:

    SNOOPY_RELEASE_VERSION_IGNORE_DIRTY
        If set, dirty working tree is not reported as such in the returned version.
        Useful for fixing up (refreshed) release tooling on-the-fly while releasing
        a package.

Usage:

    Get the software release version:
        $0

    Get the version that ignores the "-dirty" git tag:
        $0 -i

EOF
}

_showHelpAndExit()
{
    _showHelp
    exit
}



### Parse the CLI arguments
#
if [[ $@ =~ [-][-]help ]]; then
    _showHelpAndExit
fi

IGNORE_DIRTY="false"
MODE="autoreconf"

while getopts ":him:" opt; do
    case "$opt" in
        i)
            IGNORE_DIRTY="true"
            ;;

        m)
            MODE="$OPTARG"
            ;;

        h)
            _showHelpAndExit
            ;;

        ?)
            _fatalError "Unsupported argument: '-$OPTARG'. Run '$0 -h' to list supported arguments." $LINENO
            ;;

        *)
            _fatalError "Internal error (opt=$opt)" $LINENO
            ;;
    esac
done



### Parse ENV arguments
#
if [ ! -z ${SNOOPY_RELEASE_VERSION_IGNORE_DIRTY+x} ]; then
    echo "[$0] WARNING: Environment varible SNOOPY_RELEASE_VERSION_IGNORE_DIRTY set, ignoring potentially dirty working directory." 1>&2
    IGNORE_DIRTY=true
fi



### Check arguments
#
case $MODE in

    'autoreconf')
        ;;
    'git')
        ;;
    'changelog')
        ;;
    *)
        _fatalError "Invalid run mode: '$MODE'. Only 'git', 'changelog' or 'autoreconf' (default) options are supported."
        ;;
esac



#### First get current commit
##
#CURRENT_COMMIT_HASH=`git rev-parse HEAD`
#
#
#
#### Check if current commit has corresponding tag
##
#if git show-ref --tags | grep "^$CURRENT_COMMIT_HASH refs/tags" > /dev/null; then
#    CURRENT_TAG=`git show-ref --tags | grep "^$CURRENT_COMMIT_HASH refs/tags" | awk '{print $2}' | sed -e 's#^refs/tags##'`
#    CURRENT_PACKAGE_VERSION=`echo "$CURRENT_TAG" | sed -e 's/snoopy-//'`
#
#    ### Is tree dirty?
#    #
#    if git describe --tags --dirty | grep -- '-dirty$' > /dev/null; then
#        CURRENT_PACKAGE_VERSION="${CURRENT_PACKAGE_VERSION}-dirty"
#    fi
#else
#    # Here '-dirty' gets appended automatically
#    CURRENT_PACKAGE_VERSION=`git describe --tags --dirty`
#fi



### Get current Snoopy version from git, if applicable
#
# Used for building from git, and for making RC packages
#
if [ "$MODE" == "autoreconf" -o "$MODE" == "git" ]; then
    if [ -d .git ]; then
        if [ "$IGNORE_DIRTY" == "true" ]; then
            SNOOPY_RELEASE_VERSION=`git describe --tags --always | sed -e 's/^snoopy-//'`
        else
            SNOOPY_RELEASE_VERSION=`git describe --tags --always --dirty | sed -e 's/^snoopy-//'`
        fi
        echo $SNOOPY_RELEASE_VERSION
        exit 0
    fi
fi



### Otherwise get it from ChangeLog
#
# Used only if ./bootstrap.sh is run from distribution package, not from git
#
if [ "$MODE" == "autoreconf" -o "$MODE" == "changelog" ]; then
    if [ -f ChangeLog ]; then
        SNOOPY_RELEASE_VERSION=`cat ChangeLog | grep -E '^[-0-9]+ - Version [0-9]+\.[0-9]+\.[0-9]+(rc[0-9]+)?$' | head -n1 | awk '{print $4}'`
        echo $SNOOPY_RELEASE_VERSION
        exit 0
    fi
fi



### Signal error
#
_fatalError "Unable to determine Snoopy version (mode=$MODE)"
