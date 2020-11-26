#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
if ! $SNOOPY_TEST_FILTER   "msg"   "exclude_spawns_of"   "bash,aaaa,bbbb" > /dev/null; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "Message passed through when it shouldn't."
fi
