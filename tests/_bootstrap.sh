#!/bin/bash



### Configure shell
#
set -e
set -u



### Result function: PASS
#
snoopy_testResult_pass()
{
    echo "PASS"
    exit 0
}



### Result function: PASS
#
snoopy_testResult_fail()
{
    MSG="$1"
    echo "FAIL"
    echo "$1"
    exit 1
}



### Comparison function: PASS
#
snoopy_test_compareValues()
{
    VAL_SNOOPY="$1"
    VAL_REAL="$2"

    if [ "$VAL_SNOOPY" != "$VAL_REAL" ]; then
        snoopy_testResult_fail "Values do not match (snoopy=\"$VAL_SNOOPY\", real=\"$VAL_REAL\")"
    fi

    snoopy_testResult_pass
}



### Data retriever: from ps
#
snoopy_test_getValue_fromPs()
{
    PID="$1"
    FIELD="$2"

    COUNT=`ps -eo pid,$FIELD | grep -E "^[ ]*$PID[ ]+" -c`
    if [ "$COUNT" != "1" ]; then
        echo "ERROR: Invalid number of processes matched while looking for $FIELD"
        return 1
    fi

    VALUE=`ps -eo pid,$FIELD | grep -E "^[ ]*$PID[ ]+" | head -n1 | awk '{print $2}'`
    echo "$VALUE"
    return 0
}
