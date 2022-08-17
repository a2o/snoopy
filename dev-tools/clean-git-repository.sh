#!/bin/bash



### Shell configuration and script bootstrap
#
set -e
set -u
set -o pipefail
. `dirname $0`/_bootstrap.sh



### Define the help method
#
_showHelp()
{
    cat <<EOF
Purpose:

    Clean the git repository in an optimal fashion.

Supported CLI arguments:

    -h/--help   Show this help.

Usage:

    Clean the git repositry:
        $0

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

while getopts ":br:" opt; do
    case "$opt" in
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



### Check for fast exit
#
if _isGitStatusIgnoredClean ; then
    _echo "Git directory is already queaky clean."
    exit
fi



### If there is a makefile present, just use that
#
if [ -f Makefile ]; then
    make gitclean

    if _isGitStatusIgnoredClean ; then
        _echo "SUCCESS: Git directory cleaning completed (with 'make gitclean' with existing Makefile)."
        exit
    fi
fi



### If `make distclean` was already run, try to clean manually
#
# To do so, just use the configured `rm` commands in the main Makefile.am file to not duplicate the code.
#
find . -name Makefile.in -delete
cat Makefile.am | awk '/^clean-local-this-dir:/{p=1}/^$/&&p{exit}p' | tail -n+2 | source /dev/stdin
cat Makefile.am | awk '/^gitclean:/{p=1}/^$/&&p{exit}p' | tail -n+2 | source /dev/stdin

if _isGitStatusIgnoredClean ; then
    _echo "SUCCESS: Git directory cleaning completed (manually)."
    exit
fi

_echo "WARNING: Manual cleanup wasn't through enough for some reason, here are the files that remained:"
git status --ignored --short



### Do the full configuration and cleaning
#
./bootstrap.sh
./configure --enable-everything
make gitclean

if _isGitStatusIgnoredClean ; then
    _echo "SUCCESS: Git directory cleaning completed (with 'make gitclean' with a freshly generated Makefile)."
    exit
fi

_echo "ERROR: The following files weren't removed:"
git status --ignored --short
_fatalError "Unable to thoroughly clean the git directory."
