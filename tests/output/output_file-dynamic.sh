#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Output data
#
VAL_REAL=`date +%s`
MY_PID=$$
MY_USERNAME=`whoami`
FILE_EXPECTED="output_file-dynamic.sh.$MY_PID-$MY_USERNAME.tmp.out"
FILE_FORMAT="output_file-dynamic.sh.$MY_PID-%{username}.tmp.out"

# Write
rm -f $FILE_EXPECTED
$SNOOPY_TEST_CLI run output   "$VAL_REAL"   "file"   "$FILE_FORMAT" > /dev/null

# Read
VAL_SNOOPY=`cat $FILE_EXPECTED`
rm -f $FILE_EXPECTED



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
