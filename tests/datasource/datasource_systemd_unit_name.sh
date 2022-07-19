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



### Get the real value
#
VAL_EXPECTED="SNOOPY-TEST-SUITE(undefined)"
VAL_SYSTEMD_UNIT_TYPE=`cat /proc/self/cgroup | grep -E ':name=systemd:' | sed -e 's#^1:name=systemd:/##' | cut -d/ -f1`

if [ "$VAL_SYSTEMD_UNIT_TYPE" == "user.slice" ]; then
    VAL_SYSTEMD_UNIT_UID=`cat /proc/self/cgroup | grep -E ':name=systemd:' | sed -e 's#^1:name=systemd:/##' | cut -d/ -f2 | sed -e 's/^user-//' | sed -e 's/.slice//'`
    if id -nu $VAL_SYSTEMD_UNIT_UID >/dev/null 2>&1; then
        VAL_EXPECTED=`id -nu $VAL_SYSTEMD_UNIT_UID`
    else
        VAL_EXPECTED="user-$VAL_SYSTEMD_UNIT_UID"
    fi

elif [ "$VAL_SYSTEMD_UNIT_TYPE" == "system.slice" ]; then
    VAL_EXPECTED=`cat /proc/self/cgroup | grep -E ':name=systemd:' | sed -e 's#^1:name=systemd:/##' | cut -d/ -f2 | sed -e 's/.service$//'`

else
    VAL_EXPECTED=`cat /proc/self/cgroup | grep -E ':name=systemd:' | sed -e 's#^1:name=systemd:/##'`

fi



### Evaluate
#
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource systemd_unit_name`

snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
