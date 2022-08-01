#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
if $SNOOPY_TEST_CLI run filter   "exclude_spawns_of"   "aaaa" > /dev/null; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "Message unexpectedly dropped."
fi
