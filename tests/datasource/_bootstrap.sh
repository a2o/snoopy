#!/bin/bash



### Configure shell
#
set -e
set -u



### Paths
#
export SNOOPY_TEST_BIN_PREFIX=`dirname $0`/../../src/bin/snoopy-test



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
