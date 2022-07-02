#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Setup the test environment
#
export SNOOPY_TEST_LD_SO_PRELOAD_PATH="$SNOOPY_CUR_TEST_FILENAME.ld.so.preload"
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH.expected-result
echo "/another/instance/of/libsnoopy.so
/another/instance/of/libsnoopy.so" > $SNOOPY_TEST_LD_SO_PRELOAD_PATH
echo "/another/instance/of/libsnoopy.so
/another/instance/of/libsnoopy.so" > $SNOOPY_TEST_LD_SO_PRELOAD_PATH.expected-result



### Run the command & test the exit status
#
if $SNOOPY_CLI disable ; then
    snoopy_testResult_fail "Unexpected zero exit status"
fi



### Evaluate
#
if ! diff -u $SNOOPY_TEST_LD_SO_PRELOAD_PATH $SNOOPY_TEST_LD_SO_PRELOAD_PATH.expected-result ; then
    snoopy_testResult_fail "ld.so.preload test file ($SNOOPY_TEST_LD_SO_PRELOAD_PATH) content not as expected"
fi

rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH.expected-result

snoopy_testResult_pass
