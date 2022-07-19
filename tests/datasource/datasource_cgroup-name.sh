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

if ! grep -E ':name=systemd:' /proc/self/cgroup > /dev/null; then
    snoopy_testResult_skip "Cgroup :name=systemd: does not exist"
fi



### Evaluate
#
VAL_EXPECTED=`cat /proc/self/cgroup | grep -E ':name=systemd:'`
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource cgroup name=systemd`

snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
