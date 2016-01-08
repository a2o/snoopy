#!/bin/bash



### Configure shell
#
set -e
set -u

#echo ${BASH_SOURCE[1]}


### Get variables about current test
#
SNOOPY_CUR_TEST_PATH=`readlink -e $0`
SNOOPY_CUR_TEST_DIR=`dirname $SNOOPY_CUR_TEST_PATH`
SNOOPY_CUR_TEST_FILENAME=`basename $SNOOPY_CUR_TEST_PATH`
SNOOPY_CUR_TEST_NAME=`echo $SNOOPY_CUR_TEST_FILENAME | sed -e 's/\.sh$//'`
SNOOPY_CUR_TEST_GROUP_PATH=`dirname $SNOOPY_CUR_TEST_PATH`
SNOOPY_CUR_TEST_GROUP_NAME=`basename $SNOOPY_CUR_TEST_GROUP_PATH`




### Configure paths
#
SNOOPY_TESTS_ROOTDIR=`dirname $SNOOPY_CUR_TEST_GROUP_PATH`
#
# Current working directory might not be the same,
# if build is being done outside of source tree.
#
# Also now there is an assumption that all tests are placed in subdirs of
# root tests/ directory.
#
# Do not use "readlink -e $THIS_FILE_PATH" - PWD differs when building outside of source dir
#
SNOOPY_CUR_TEST_WORKDIR=`pwd`
SNOOPY_TESTS_BINDIR="$SNOOPY_CUR_TEST_WORKDIR/../bin"
SNOOPY_TESTS_BIN_PREFIX="$SNOOPY_TESTS_BINDIR/snoopy-test"



### Configure paths to test helpers
#
export SNOOPY_TEST_CONFIGFILE="${SNOOPY_TESTS_BIN_PREFIX}-configfile"
export SNOOPY_TEST_DATASOURCE="${SNOOPY_TESTS_BIN_PREFIX}-datasource"
export SNOOPY_TEST_FILTER="${SNOOPY_TESTS_BIN_PREFIX}-filter"
export SNOOPY_TEST_MESSAGE_FORMAT="${SNOOPY_TESTS_BIN_PREFIX}-message-format"
export SNOOPY_TEST_OUTPUT="${SNOOPY_TESTS_BIN_PREFIX}-output"
export SNOOPY_TEST_EXEC_IN_THREADS="${SNOOPY_TESTS_BIN_PREFIX}-exec-in-threads"
export SNOOPY_TEST_THREADS="${SNOOPY_TESTS_BIN_PREFIX}-threads"
export SNOOPY_LIBSNOOPY_TEST_SO_PATH="${SNOOPY_TESTS_BINDIR}/.libs/libsnoopy-test.so"



### Test run function: info output
#
snoopy_testRun_info()
{
    echo "$1"
}



### Result function: PASS
#
snoopy_testResult_pass()
{
    echo "PASS"
    if [ ! -z "${1:-}" ]; then
        echo "$1"
    fi
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



### Result function: SKIP
#
snoopy_testResult_skip()
{
    MSG="$1"
    echo "SKIPPED: $MSG"
    exit 77
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
