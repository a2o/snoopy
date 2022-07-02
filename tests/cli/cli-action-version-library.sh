#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Test for error(s)
#
SEARCH_REGEX="^Snoopy library version:  [-.a-z0-9]+"
if ! $SNOOPY_CLI version | grep -E "$SEARCH_REGEX" > /dev/null ; then
    snoopy_testResult_fail "Unable to find expected version information: $SEARCH_REGEX"
fi



### Success
#
snoopy_testResult_pass
