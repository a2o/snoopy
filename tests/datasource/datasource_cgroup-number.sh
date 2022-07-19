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

if ! grep -E '^1:' /proc/self/cgroup > /dev/null; then
    snoopy_testResult_skip "Cgroup 1: does not exist"
fi



### Evaluate
#
VAL_EXPECTED=`cat /proc/self/cgroup | grep -E '^1:'`
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource cgroup 1`

snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
