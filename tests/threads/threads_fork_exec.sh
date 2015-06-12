#!/bin/bash



### Configure shell and bootstrap
#
#set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="1"
THREAD_COUNT="100"
THREAD_EXEC_CMD="/bin/hostname"
VAL_SNOOPY=`$SNOOPY_TEST_EXEC_IN_THREADS  $THREAD_COUNT  $THREAD_EXEC_CMD  | grep '^SUCCESS' -c | cat`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
