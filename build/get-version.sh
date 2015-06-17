#!/bin/bash



### Description
#
# Expected output:
#   1. 2.3.1
#   2. 2.3.1-dirty
#   3. snoopy-2.3.1-146-gCOMMIT
#   4. snoopy-2.3.1-146-gCOMMIT-dirty
#
# Options 1. and 2. are presented when current commit corresponds to explicit git tag.
# Options 3. and 4. are for other occasions.
# If tree is dirty, then add "-dirty" suffix.
#



### Configure shell first
#
set -e
set -u



### Check arguments
#
MODE=${1:-all}
case $MODE in

    'all')
        ;;
    'git')
        ;;
    'changelog')
        ;;

    *)
        echo "ERROR: Invalid mode: $MODE"
        exit 1
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
if [ "$MODE" == "all" -o "$MODE" == "git" ]; then
    if [ -d .git ]; then
        CURRENT_PACKAGE_VERSION=`git describe --tags --dirty | sed -e 's/^snoopy-//'`
        echo $CURRENT_PACKAGE_VERSION
        exit 0
    fi
fi



### Otherwise get it from ChangeLog
#
# Used only if ./bootstrap.sh is run from distribution package, not from git
#
if [ "$MODE" == "all" -o "$MODE" == "changelog" ]; then
    if [ -f ChangeLog ]; then
        CURRENT_PACKAGE_VERSION=`cat ChangeLog | grep -E '^[-0-9]+ - Version [.0-9]+$' | head -n1 | awk '{print $4}'`
        echo $CURRENT_PACKAGE_VERSION
        exit 0
    fi
fi



### Signal error
#
echo "Unable to determine Snoopy version (mode=$MODE)!"
exit 1
