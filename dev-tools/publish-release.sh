#!/bin/bash



### How does this script operate?
#
# Script's expectations:
# - 
# - Executed from the root of the Snoopy's git repository working directory
# - Release commit already present and tagged in the repo
# - Release commit/tag checked out in the current working directory
# - Release package(s) already present in the directory (created via ./dev-tools/build-package-*.sh scripts)
#
# Script's arguments:
# - Release tag to publish (must match currently checked out commit)
#
# Steps that this script performs:
# - Verifies the release commit+tag presence in local repo and being checked out
# - Determines the remote to use (from a current branch), overridable
# - Push release commit + tag to remote
# - Creates a release via GitHub API
# - Pushes package file(s) to the release via GitHub API



### Shell configuration and script bootstrap
#
set -e
set -u
. `dirname $0`/_bootstrap.sh



### Configuration defaults
#
SNOOPY_UPSTREAM_GIT_REPO_REGEX="(git@github.com:|https://github.com/)a2o/snoopy(\.git)?(/)?"



### Define the help method
#
_showHelp()
{
    cat <<EOF
Purpose:

    Publish built release package(s) as a GitHub release.

Supported CLI arguments:

    -c          Use the currently checked-out branch for publishing.
                By default, using the checked out "master" branch is expected, and
                that branch should track https://github.com/a2o/snoopy's master branch.
                If another branch is currently checked out, it must not be tracking
                the https://github.com/a2o/snoopy repository.

    -y          Confirm the intention.
                Skips the confirmation question.

    -t TAG      Git tag to publish release for.
                Defaults to a tag as returned by ./dev-tools/libexec/get-release-tag.sh for the currently checked-out commit.

    -d          Allow "dirty" working directory.
                By default, this tool requires all changes to be committed.

    -r REMOTE   Name of configured remote to use for publishing.
                Defaults to the name that matches the "git@github.com:a2o/snoopy(.git)?(/)?" URL.

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
ALLOW_DIRTY="false"
RELEASE_TAG_CLI=""
CONFIRMED_VIA_CLI="false"

while getopts ":ye:cdt:m:snr:h" opt; do
    case "$opt" in
        c)
            SKIP_CURRENT_BRANCH_CHECK="true"
            ;;

        d)
            ALLOW_DIRTY="true"
            ;;

        t)
            RELEASE_TAG_CLI="$OPTARG"
            ;;

        y)
            CONFIRMED_VIA_CLI="true"
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



### Check if there are uncommitted changes in this repository
#
if [ "$ALLOW_DIRTY" == "false" ]; then
    RES=`git status --short | grep -c .`
    if [ "$RES" != "0" ]; then
        _fatalError "There are uncommitted changes in this repository (the '-d' flag skips this check)" $LINENO
    fi
fi



### Check if we're on the master branch
#
CURRENT_GIT_BRANCH=`git branch --show-current`
if [ "$SKIP_CURRENT_BRANCH_CHECK" == "false" ]; then
    if [ "$CURRENT_GIT_BRANCH" != "master" ]; then
        _fatalError "Not on the 'master' git branch (the '-c' flag skips this check)" $LINENO
    fi
fi
_echo "Using currently checked out git branch: $CURRENT_GIT_BRANCH"



### Get the remote tracking branch, and the corresponding remote's URL
#
GIT_REMOTE_NAME=`git rev-parse --abbrev-ref --symbolic-full-name @{u} | cut -d/ -f1`
if [ "$GIT_REMOTE_NAME" == "" ]; then
    _fatalError "Current branch '$CURRENT_GIT_BRANCH' is not tracking any remote branch. This is an error, as the publishing target repository is determined by the remote branch the currently checked out branch is tracking." $LINENO
fi
GIT_REMOTE_URL=`git remote -v | grep -E "^$GIT_REMOTE_NAME" | grep -E ' \(push\)$' | awk '{print $2}'`
if [ "$GIT_REMOTE_URL" == "" ]; then
    _fatalError "Unable to determine the target URL of the git remote named '$GIT_REMOTE_NAME'" $LINENO
fi
GIT_REMOTE_BRANCH_NAME=`git rev-parse --abbrev-ref --symbolic-full-name @{u} | sed -e "s#^$GIT_REMOTE_NAME/##"`
if [ "$CURRENT_GIT_BRANCH" != "$GIT_REMOTE_BRANCH_NAME" ]; then
    _fatalError "This script does not support mismatching local and remote branch names (local:$CURRENT_GIT_BRANCH, remote:$GIT_REMOTE_BRANCH_NAME)." $LINENO
fi



### Check the release target sanity
#
# If the current branch is 'master', the tracked remote should be Snoopy's upstream repository
if [ "$CURRENT_GIT_BRANCH" == "master" ]; then
    if ! [[ $GIT_REMOTE_URL =~ $SNOOPY_UPSTREAM_GIT_REPO_REGEX ]]; then
        _fatalError "Currently checked out branch 'master' is not tracking the Snoopy's upstream repository. This is most likely an error - releasing stable Snoopy via a non-primary upstream repository?" $LINENO
    fi
fi

# If the current branch is NOT 'master', the tracked remote should NOT be Snoopy's upstream repository
if [ "$CURRENT_GIT_BRANCH" != "master" ]; then
    if [[ $GIT_REMOTE_URL =~ $SNOOPY_UPSTREAM_GIT_REPO_REGEX ]]; then
        _fatalError "Currently checked out branch '$CURRENT_GIT_BRANCH' is tracking the Snoopy's upstream repository. This is most likely an error - releasing a non-stable Snoopy via the primary upstream repository?" $LINENO
    fi
fi
GIT_REMOTE_GITHUB_ID=`echo "$GIT_REMOTE_URL" | sed -e 's#^git@github.com:##' | sed -e 's#^https://github.com//##' | sed -e 's#\.git/\?$##'`
_echo "Using git remote '$GIT_REMOTE_NAME' for publishing (url=$GIT_REMOTE_URL, ghId=$GIT_REMOTE_GITHUB_ID)."



### Determine the release tag to work with
#
RELEASE_TAG=""
if [ "$RELEASE_TAG_CLI" != "" ]; then
    if ! _isReleaseTagFormatValid "$RELEASE_TAG_CLI"; then
        _fatalError "Invalid format of a release tag argument: $RELEASE_TAG_CLI" $LINENO
    fi
    RELEASE_TAG="$RELEASE_TAG_CLI"
    _echo "Got release tag from CLI argument: $RELEASE_TAG"
else
    RELEASE_TAG=`./dev-tools/libexec/get-release-tag.sh git`
    _echo "Got release tag from git history: $RELEASE_TAG"
fi
RELEASE_VERSION=`echo "$RELEASE_TAG" | sed -e 's/snoopy-//'`



### Check if the release tag and commit message are present on the local branch
#
# Tag
if ! git log --pretty="%h %d %s" | grep -E "(\(|, )tag: $RELEASE_TAG(\)|, )" > /dev/null; then
    _fatalError "Git tag '$RELEASE_TAG' not found on the current git branch." $LINENO
fi

# Commit message
if ! git log --pretty="%h %d %s" | grep -E "(\(|, )tag: $RELEASE_TAG(\)|, )" | grep -E "\) Release $RELEASE_VERSION\$" > /dev/null; then
    _fatalError "Git tag '$RELEASE_TAG' does not seem to be representing a release commit." $LINENO
fi
RELEASE_COMMIT_ID=`git log --format="%h" $RELEASE_TAG | head -n1`
_echo "Release tag and commit ($RELEASE_COMMIT_ID) seem to be in order locally."



### Check if the release commit and tag already exists in remote, else push them
#
# Fetch
_echo "Fetching from remote '$GIT_REMOTE_NAME'..."
git fetch $GIT_REMOTE_NAME

# Commit
GIT_PUSH_COMMITS="false"
if git branch -r --contains $RELEASE_COMMIT_ID | grep -E "$GIT_REMOTE_NAME/$CURRENT_GIT_BRANCH\$" > /dev/null; then
    _echo "Remote branch $GIT_REMOTE_NAME/$CURRENT_GIT_BRANCH already contains commit $RELEASE_COMMIT_ID, push will be skipped."
else
    _echo "Will push commits from local branch '$CURRENT_GIT_BRANCH' to remote '$GIT_REMOTE_NAME'."
    GIT_PUSH_COMMITS="true"
fi

# Tag
GIT_PUSH_TAG="false"
if git ls-remote --tags origin | grep -E "refs/tags/$RELEASE_TAG\$" > /dev/null; then
    # Check if the remote tag corresponds to our commit ID
    if ! git ls-remote --tags origin | grep -E "^$RELEASE_COMMIT_ID.+refs/tags/$RELEASE_TAG\$" > /dev/null; then
        _fatalError "Git tag '$RELEASE_TAG' already exists in the remote '$GIT_REMOTE_NAME', but it does not represent the release commit $RELEASE_COMMIT_ID." $LINENO
    fi
    _echo "Remote '$GIT_REMOTE_NAME' already contains tag $RELEASE_TAG, push will be skipped."
else
    _echo "Will push tag '$RELEASE_TAG' to remote '$GIT_REMOTE_NAME'."
    GIT_PUSH_TAG="true"
fi



### Check the presence of the to-be-published packages
#
RELEASE_PACKAGE_TARGZ="$RELEASE_TAG.tar.gz"
if [ ! -f $RELEASE_PACKAGE_TARGZ ]; then
    _fatalError "Unable to find local package to release: $RELEASE_PACKAGE_TARGZ" $LINENO
fi



### Check if access to GitHub API is working (via the "gh" CLI tool)
#
if ! which gh > /dev/null; then
    _fatalError "Unable to GitHub CLI tool called 'gh'." $LINENO
fi
if ! gh release list -R $GIT_REMOTE_GITHUB_ID > /dev/null; then
    _fatalError "GitHub CLI access is not working. Run 'gh release list -R $GIT_REMOTE_GITHUB_ID' to see the actual error." $LINENO
fi
RES=`gh release list -R $GIT_REMOTE_GITHUB_ID | grep -P "^$RELEASE_TAG[ \t]+(Latest[ \t]+)?$RELEASE_TAG" -c | cat`
if [ "$RES" != "0" ]; then
    _fatalError "Release $RELEASE_TAG already exists in the $GIT_REMOTE_GITHUB_ID git repository." $LINENO
fi
_echo "GitHub API access works, and release $RELEASE_TAG does not exist yet in $GIT_REMOTE_GITHUB_ID repo, good."



### Sanity check complete, double check the intent
#
_echo "All checks successful, starting the releasing process."
_echo "Target remote name:      $GIT_REMOTE_NAME"
_echo "Target remote URL:       $GIT_REMOTE_URL"
_echo "Target remote GitHub id: $GIT_REMOTE_GITHUB_ID"

if [ "$CONFIRMED_VIA_CLI" != "true" ]; then
    _echo "Are you sure you want to proceed?"
    INPUT=""
    while [ "$INPUT" != "yes" ]; do
        _echo "(Enter 'yes' or rerun the command with the '-y' flag) "
        read INPUT
    done
fi



### Push git content
#
if [ "$GIT_PUSH_COMMITS" == "true" ]; then
    _echo "Pushing commits from local branch '$CURRENT_GIT_BRANCH' to remote '$GIT_REMOTE_NAME':"
    git push
    _echo "Commits pushed."
fi

if [ "$GIT_PUSH_TAG" == "true" ]; then
    _echo "Pushing tag '$RELEASE_TAG' to remote '$GIT_REMOTE_NAME'..."
    git push $GIT_REMOTE_NAME $RELEASE_TAG
    _echo "Tags pushed."
fi



### Create a release
#
_echo "Creating a release:"
gh release create \
    -R $GIT_REMOTE_GITHUB_ID \
    -n "" \
    -t "$RELEASE_TAG" \
    "$RELEASE_TAG" \
    $RELEASE_PACKAGE_TARGZ
_echo ""
_echo "SUCCESS: Snoopy version $RELEASE_VERSION released."
_echo "SUCCESS: The new release is available at:"
_echo "SUCCESS:"
_echo "SUCCESS:     https://github.com/$GIT_REMOTE_GITHUB_ID/releases/tag/$RELEASE_TAG"
_echo ""
