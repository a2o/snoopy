#!/bin/bash



#!/bin/bash



### How does this script operate?
#
# Script's expectations:
# - A directory that contains IMPORT.defs, where the import definitions are stored
# - Access to the referenced git repository URL (via internet, or otherwise)
#
# Script's arguments:
# - Path to the directory containing the external library to import/update
#
# Steps that this script performs:
# - Reads the external library import definitions (from the IMPORT.defs file)
# - Clones the library's git repo to a temporary location
# - Executes the import routine
# - Applies patches, if any are found in the patches/ directory inside target directory
# - In the IMPORT.defs: updates the git reference that was used for the import



### Shell configuration and script bootstrap
#
set -e
set -u
. `dirname $0`/_bootstrap.sh



### Define the help method
#
_showHelp()
{
    cat <<EOF
Purpose:

    Import/update an external library (in lib/) directory

Supported CLI arguments:

    -r          Git reference to use for import. It can be anything that uniquely
                identifies a commit in the external library's git repository.
                By default, the last commit on the repository's default branch is
                used.
                (Incompatible with -c flag.)

    -c          Use the current git reference that was used for the last import.
                Mainly useful for testing the import procedure.
                (Incompatible with -r option.)

    -k          Keep for inspection the git clone directory that was created during
                the import.

    -h/--help   Show this help.

Usage:

    Import the latest version (potentially non-stable):
        $0 lib/inih

    Import a specific commit/tag:
        $0 -r r42 lib/inih

    Re-import the currently imported commit/tag:
        $0 -c lib/inih

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

ARG_GIT_REF=""
ARG_GIT_REF_USE_CURRENT="false"
ARG_KEEP_GIT_CLONE="false"

while getopts ":r:ckh" opt; do
    case "$opt" in
        r)
            ARG_GIT_REF="$OPTARG"
            ;;

        c)
            ARG_GIT_REF_USE_CURRENT="true"
            ;;

        k)
            ARG_KEEP_GIT_CLONE="true"
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

if [[ "$ARG_GIT_REF" != "" ]] && [[ "$ARG_GIT_REF_USE_CURRENT" == "true" ]]; then
    _fatalError "The -r and -c flags cannot be used at the same time. Run '$0 -h' for more information." $LINENO
fi

RES="${@:$OPTIND:1}"
if [ "$RES" == "" ]; then
    _fatalError "Missing the import target. Usage example: $0 PATH/TO/LIB" $LINENO
fi
ARG_EXTLIB_DIR="$RES"



### Check the target directory
#
if [ ! -d $ARG_EXTLIB_DIR ]; then
    _fatalError "Import target directory missing: $ARG_EXTLIB_DIR" $LINENO
fi
EXTLIB_DIR="$ARG_EXTLIB_DIR"



### Read the import definitions
#
EXTLIB_IMPORT_DEFS_FILE="$EXTLIB_DIR/IMPORT.defs"
if [ ! -f $EXTLIB_IMPORT_DEFS_FILE ]; then
    _fatalError "Missing import definitions file: $EXTLIB_IMPORT_DEFS_FILE" $LINENO
fi
_echo "Reading the import definitions file: $EXTLIB_IMPORT_DEFS_FILE"
. $EXTLIB_IMPORT_DEFS_FILE



### Decide on the final ref to use
#
EXTLIB_GIT_REF_TO_USE=""
if [ "$ARG_GIT_REF" != "" ]; then
    EXTLIB_GIT_REF_TO_USE="$ARG_GIT_REF"
    _echo "Will use the following git reference for import: $EXTLIB_GIT_REF_TO_USE"
elif [ "$ARG_GIT_REF_USE_CURRENT" == "true" ]; then
    EXTLIB_GIT_REF_TO_USE="$EXTLIB_GIT_REF"
    _echo "Will use the currently-used git reference for import: $EXTLIB_GIT_REF_TO_USE"
else
    _echo "No git reference for import was specified, will use the last commit on the default branch."
fi



### Clone the repo
#
EXTLIB_DIR_GIT_TMP="$EXTLIB_DIR/_import-git-repo"
rm -rf $EXTLIB_DIR_GIT_TMP
_echo "Cloning the git repository: $EXTLIB_GIT_REPO_URL"
git clone $EXTLIB_GIT_REPO_URL $EXTLIB_DIR_GIT_TMP

if [ "$EXTLIB_GIT_REF_TO_USE" != "" ]; then
    _echo "Checking out git reference: $EXTLIB_GIT_REF_TO_USE"
    (
        cd $EXTLIB_DIR_GIT_TMP
        git checkout --detach $EXTLIB_GIT_REF_TO_USE
    )
fi



### Run the import
#
_echo "Running the import..."
set +x
_snoopy_extlib_import "$EXTLIB_DIR_GIT_TMP" "$EXTLIB_DIR"
_echo "Import complete."



### Apply the patches
#
EXTLIB_DIR_PATCHES="$EXTLIB_DIR/patches"
if [ -d $EXTLIB_DIR_PATCHES ]; then
    _echo "Applying patches:"
    (
        cd $EXTLIB_DIR
        for PATCHFILE in `find . -type f -regex '.+\.\(diff\|patch\)$' | sort`; do
            _echo "  Applying patch $PATCHFILE..."
            patch -p0 < $PATCHFILE
            _echo "  Applying patch $PATCHFILE done."
        done
    )
    _echo "All patches applied."
fi



### Update the git reference that was used for import
#
if [ "$EXTLIB_GIT_REF_TO_USE" != "$EXTLIB_GIT_REF" ]; then
    EXTLIB_GIT_REF_TO_STORE=`cd $EXTLIB_DIR_GIT_TMP && git describe --all --long --always`
    _echo "Storing git reference that was used for import: $EXTLIB_GIT_REF_TO_STORE"
    _echo "Storing git reference that was used for import in: $EXTLIB_IMPORT_DEFS_FILE"
    sed -i "s#^EXTLIB_GIT_REF=.*#EXTLIB_GIT_REF=\"$EXTLIB_GIT_REF_TO_STORE\"#" $EXTLIB_IMPORT_DEFS_FILE
fi



### Remove the tmp git dir
#
if [ "$ARG_KEEP_GIT_CLONE" == "true" ]; then
    _echo "Keeping the temporary git clone around for your inspection, at: $EXTLIB_DIR_GIT_TMP"
else
    rm -rf $EXTLIB_DIR_GIT_TMP
fi



### All done.
#
_echo "Import complete."
_echo "(Use 'git diff $EXTLIB_DIR' to inspect the changes.)"
