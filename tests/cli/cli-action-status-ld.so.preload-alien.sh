#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Prepare the environment
#
export SNOOPY_TEST_LD_SO_PRELOAD_PATH="$SNOOPY_CUR_TEST_FILENAME.ld.so.preload"
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH
echo "/unknown/lib/$SNOOPY_TEST_LIBSNOOPY_SO_PATH" > $SNOOPY_TEST_LD_SO_PRELOAD_PATH



### Execute & verify
#
if ! $SNOOPY_CLI status 2>&1 | grep -E "^/etc/ld.so.preload: +NOT OK - 'libsnoopy.so' found, but not with the expected path" > /dev/null ; then
    snoopy_testResult_fail "ld.so.preload contains invalid libsnoopy.so reference, but status error message not as expected"
fi



### Success
#
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH
snoopy_testResult_pass
