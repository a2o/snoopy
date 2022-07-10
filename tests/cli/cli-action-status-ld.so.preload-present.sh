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
echo "$SNOOPY_TEST_LIBSNOOPY_SO_PATH" > $SNOOPY_TEST_LD_SO_PRELOAD_PATH



### Execute & verify
#
if ! $SNOOPY_CLI status 2>&1 | grep -E '^/etc/ld.so.preload: +OK - Snoopy is enabled.' > /dev/null ; then
    snoopy_testResult_fail "ld.so.preload contains valid libsnoopy.so reference, but status says otherwise"
fi



### Success
#
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH
snoopy_testResult_pass
