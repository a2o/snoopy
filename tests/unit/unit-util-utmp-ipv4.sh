#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Run the mock test
#
VAL_EXPECTED="89.16.143.91"
VAL_SNOOPY=`$SNOOPY_TEST_CLI unit util utmp /dev/pts/1 $SNOOPY_CUR_TEST_DIR/unit-util-utmp-testUtmpFile`

snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
