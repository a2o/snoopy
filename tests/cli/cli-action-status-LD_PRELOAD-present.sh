#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Prepare the environment
#
export SNOOPY_TEST_LD_SO_PRELOAD_PATH="$SNOOPY_CUR_TEST_FILENAME.ld.so.preload"
export LD_PRELOAD="$SNOOPY_TEST_LIBSNOOPY_SO_PATH"



### Execute & verify
#
if ! $SNOOPY_CLI status 2>&1 | grep -E '^LD_PRELOAD environment var: +OK - Snoopy reference found.' > /dev/null ; then
    snoopy_testResult_fail "LD_PRELOAD contains valid libsnoopy.so reference, but status says otherwise"
fi



### Success
#
snoopy_testResult_pass
