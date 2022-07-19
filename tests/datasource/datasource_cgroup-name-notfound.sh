#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Verify the environment
#
if [ ! -f /proc/self/cgroup ]; then
    snoopy_testResult_skip "File /proc/self/cgroup does not exist"
fi



### Evaluate
#
VAL_EXPECTED="(none)"
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource cgroup randomCgroupNameThatDefinitelyDoesntExist`

snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
