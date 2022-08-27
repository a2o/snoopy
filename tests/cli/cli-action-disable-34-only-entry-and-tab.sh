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
echo "$SNOOPY_TEST_LIBSNOOPY_SO_PATH	# comment" > $SNOOPY_TEST_LD_SO_PRELOAD_PATH



### Run the command
#
$SNOOPY_CLI disable



### Evaluate
#
if [ ! -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH ]; then
    snoopy_testResult_fail "ld.so.preload test file ($SNOOPY_TEST_LD_SO_PRELOAD_PATH) unexpectedly missing"
fi

FILESIZE=`stat -c "%s" $SNOOPY_TEST_LD_SO_PRELOAD_PATH`
if [ "$FILESIZE" != "0" ]; then
    snoopy_testResult_fail "ld.so.preload test file ($SNOOPY_TEST_LD_SO_PRELOAD_PATH) unexpectedly non-empty"
fi

rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH
snoopy_testResult_pass
