#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE rpname`

# Get root process PID
CUR_PID=$$
NEW_PPID=`snoopy_test_getValue_fromPs "$CUR_PID" "ppid"`
I=0
while [ "$NEW_PPID" != "1" ]; do
    I=`expr $I + 1`
    if [ "$I" -gt "100" ]; then
        snoopy_testResult_fail "Endless loop".
    fi

    CUR_PID="$NEW_PPID"
    NEW_PPID=`snoopy_test_getValue_fromPs "$NEW_PPID" "ppid"`
done

# Get root parent process name
VAL_REAL=`cat /proc/$CUR_PID/status | grep '^Name:' | awk '{print $2}'`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
