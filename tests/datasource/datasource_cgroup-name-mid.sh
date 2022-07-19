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

if ! grep -E '^[0-9]:[^,:]+,[^,:]+,[^:]+:' /proc/self/cgroup > /dev/null; then
    snoopy_testResult_skip "Cgroup with more than two controllers does not exist"
fi



### Evaluate
#
CONTROLLER_NAME=`cat /proc/self/cgroup | grep -E '^[0-9]:[^,:]+,[^,:]+,[^:]+:' | head -n1 | cut -d: -f2 | cut -d, -f2`
VAL_EXPECTED=`cat /proc/self/cgroup | grep -E "^[0-9]:[^,:]+,$CONTROLLER_NAME," | head -n1`
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource cgroup "$CONTROLLER_NAME"`

snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
