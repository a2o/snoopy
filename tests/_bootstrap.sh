#!/bin/bash



### Configure shell
#
set -e
set -u



### Configure paths
#
# Get own location
THIS_FILE="$BASH_SOURCE"
THIS_FILE_PATH=`readlink -e $THIS_FILE`
#
# Dirs
export SNOOPY_TESTS_ROOTDIR=`dirname $THIS_FILE_PATH`
export SNOOPY_TESTS_BINDIR="$SNOOPY_TESTS_ROOTDIR/bin"
#
# Test suite helpers
export SNOOPY_TEST_BIN_PREFIX="$SNOOPY_TESTS_BINDIR/snoopy-test"
export SNOOPY_TEST_CONFIGFILE="${SNOOPY_TEST_BIN_PREFIX}-configfile"
export SNOOPY_TEST_DATASOURCE="${SNOOPY_TEST_BIN_PREFIX}-datasource"
export SNOOPY_TEST_FILTER="${SNOOPY_TEST_BIN_PREFIX}-filter"
export SNOOPY_TEST_MESSAGE_FORMAT="${SNOOPY_TEST_BIN_PREFIX}-message-format"
export SNOOPY_TEST_OUTPUT="${SNOOPY_TEST_BIN_PREFIX}-output"
#
# Currently executing test filename
export SNOOPY_CUR_TEST_FILENAME=`basename $0`



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
