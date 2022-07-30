#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
if "$SNOOPY_TESTS_BINDIR/space parent" $SNOOPY_TEST_CLI run filter   "exclude_spawns_of"   "parachuteparent"; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "Message passed through when it shouldn't."
fi
