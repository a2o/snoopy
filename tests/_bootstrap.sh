#!/bin/bash



### Configure shell
#
set -e
set -u



### Source build-based variables
#
. ../_bootstrap-paths.sh



### Get variables about current test
#
SNOOPY_CUR_TEST_PATH=`readlink -f $0`
echo "Current test path: $SNOOPY_CUR_TEST_PATH"
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
# Do not use "readlink -f $THIS_FILE_PATH" - PWD differs when building outside of source dir
#
SNOOPY_CUR_TEST_WORKDIR=`pwd`
SNOOPY_TESTS_BINDIR="$SNOOPY_CUR_TEST_WORKDIR/../bin"
SNOOPY_TEST_CLI="$SNOOPY_TESTS_BINDIR/snoopy-test"
export SNOOPY_LIBSNOOPY_TEST_SO_PATH="${SNOOPY_TESTS_BINDIR}/.libs/libsnoopy-test-execve-wrapper-env-ini-path.so"



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
