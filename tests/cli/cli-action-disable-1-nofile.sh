#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Setup the test environment
#
export SNOOPY_TEST_LD_SO_PRELOAD_PATH="$SNOOPY_CUR_TEST_FILENAME.ld.so.preload"
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH



### Run the command
#
$SNOOPY_CLI disable



### Evaluate
#
if [ -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH ]; then
    snoopy_testResult_fail "ld.so.preload test file ($SNOOPY_TEST_LD_SO_PRELOAD_PATH) unexpectedly appeared"
fi

snoopy_testResult_pass
