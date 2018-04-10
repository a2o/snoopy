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
# Stop at:
# - PID 1: init
# - PID 0: appears in container when attached to it from the host
while [[ "$NEW_PPID" != "1" ]] && [[ "$NEW_PPID" != "0" ]]; do
    I=`expr $I + 1`
    if [ "$I" -gt "100" ]; then
        snoopy_testResult_fail "Endless loop".
    fi

    CUR_PID="$NEW_PPID"
    NEW_PPID=`snoopy_test_getValue_fromPs "$NEW_PPID" "ppid"`
done

# Get root parent process name
VAL_REAL=`cat /proc/$CUR_PID/status | grep '^Name:' | sed 's/^Name:\t//'`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
