#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Test for error(s)
#
if $SNOOPY_CLI sampleUnknownAction > /dev/null 2>&1 ; then
    snoopy_testResult_fail "Expected non-zero exit status"
fi



### Success
#
snoopy_testResult_pass
