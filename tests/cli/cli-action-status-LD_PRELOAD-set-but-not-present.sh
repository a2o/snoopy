#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Prepare the environment
#
export SNOOPY_TEST_LD_SO_PRELOAD_PATH="$SNOOPY_CUR_TEST_FILENAME.ld.so.preload"
export LD_PRELOAD=""



### Execute & verify
#
if ! $SNOOPY_CLI status 2>&1 | grep -E '^LD_PRELOAD environment var: +NOT SET - Set, but no Snoopy reference found' > /dev/null ; then
    snoopy_testResult_fail "LD_PRELOAD set but does not contain a valid libsnoopy.so reference, yet status says otherwise"
fi



### Success
#
rm -f $SNOOPY_TEST_LD_SO_PRELOAD_PATH
snoopy_testResult_pass
