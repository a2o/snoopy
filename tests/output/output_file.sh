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
FILE="output_file.sh.$MY_PID.tmp.out"

# Write
rm -f $FILE
$SNOOPY_TEST_OUTPUT   "$VAL_REAL"   "file"   "$FILE" > /dev/null

# Read
VAL_SNOOPY=`cat $FILE`
rm -f $FILE



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
