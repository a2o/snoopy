#!/bin/bash



### How does this script operate?
#
# Script's expectations:
# - `ChangeLog` file already updated, containing the version-to-be-released at the top
# - `README.md` file already updated, containing references to version-to-be-released in the
#     `News` and `Latest version` sections
# - The edits  to `ChangeLog` and `README.md` not yet committed, but can already be staged
# - The commit containing message `Release X.Y.Z` not existing in the git history
# - The tag `snoopy-x.y.z` not existing in the git repository
#
# Script's arguments:
# - (none)
#
# Steps that this script performs:
# - A check that we're on the `master` branch (can be avoided with the `-c` flag)
# - Gets the release tag from the ChangeLog (via `./dev-tools/libexec/get-release-tag.sh changelog`)
# - Verifies ChangeLog content
# - Verifies README.md content (`News` and `Latest version` sections)
# - Creates an appropriately formatted commit (including sign-off message)



### Shell configuration and script bootstrap
#
set -e
set -u
. `dirname $0`/_bootstrap.sh



### Define help method
#
_showHelp()
{
    cat <<EOF
Purpose:

    Create a git commit and tag for a release

Supported CLI arguments:

    -c          Use current branch for the commit, regardless of what it is.
                By default, the "master" branch needs to be checked out.

    -h/--help   Show this help.

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

SKIP_CURRENT_BRANCH_CHECK="false"
while getopts "ch" opt; do
    case "$opt" in
        c)
            SKIP_CURRENT_BRANCH_CHECK="true"
            ;;

        h)
            _showHelpAndExit
            ;;

        ?)
            _fatalError "Unsupported argument: '-$OPTARG'. Run '$0 -h' to list supported arguments."
            ;;

        *)
            _fatalError "Internal error (opt=$opt)"
            ;;
    esac
done



### Check if we're on the master branch
#
CURRENT_GIT_BRANCH=`git branch --show-current`
if [ "$SKIP_CURRENT_BRANCH_CHECK" == "false" ]; then
    if [ "$CURRENT_GIT_BRANCH" != "master" ]; then
        _fatalError "Not on the 'master' git branch (the '-c' flag skips this check)" $LINENO
    fi
fi



### Get the release tag from ChangeLog file
#
_echo "Getting release tag from ChangeLog..."
RELEASE_TAG=`./dev-tools/libexec/get-release-tag.sh -m changelog`
_echo "Got release tag: $RELEASE_TAG"
RELEASE_VERSION=`echo $RELEASE_TAG | sed -e 's/^snoopy-//'`
_echo "Got release version: $RELEASE_VERSION"



### Check git history for the existence of a commit
#
COMMIT_MESSAGE="Release $RELEASE_VERSION"
if git log --pretty=format:"%s" | egrep "^$COMMIT_MESSAGE\$" > /dev/null; then
    _fatalError "Git commit with message'$COMMIT_MESSAGE' already exists" $LINENO
fi



### Check git history for the existence of a tag
#
if git tag | egrep "^$RELEASE_TAG\$" > /dev/null; then
    _fatalError "Git tag '$RELEASE_TAG' already exists" $LINENO
fi



### Verify last version information
#
if ! ./dev-tools/libexec/verify-last-version-everywhere.sh "$RELEASE_VERSION"; then
    _fatalError "Last version information is not consistent everywhere: $RELEASE_VERSION" $LINENO
fi
_echo "Last release version information is consistent everywhere."



### Verify if ChangeLog and README.md changes are not yet committed
#
if ! git status | grep 'modified:' | grep 'ChangeLog$' > /dev/null; then
    _fatalError "Release-related ChangeLog changes must not yet be committed - they must a part of the release commit" $LINENO
fi
if ! git status | grep 'modified:' | grep 'README.md$' > /dev/null; then
    _fatalError "Release-related README.md changes must not yet be committed - they must a part of the release commit" $LINENO
fi



### Create a git commit and tag
#
# - Pre-format the commit message
# - Open the editor for any additional tweaking
# - If the commit is successful, tag the commit
#
git add ChangeLog
git add README.md
git commit -m "Release $RELEASE_VERSION

#
#
# If you want to terminate the commit+tag process here,
# comment out all the lines above and save.
# This will result in the 'Aborting commit due to empty commit message' error.
#
#
#
" -e -s -v
git tag "$RELEASE_TAG"



### Report success
#
_echo ""
_echo "SUCCESS: Commit and tag for version $RELEASE_VERSION created (locally)."
_echo ""



### Suggest next step(s)
#
_echo "
Next step:
==========

4. Build the release package:

    ./dev-tools/build-release-package.sh

"
