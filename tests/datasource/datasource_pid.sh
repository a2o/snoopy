#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#

# Run this command first, in background
$SNOOPY_TEST_DATASOURCE pid > datasource_pid.out.tmp &

# Capture its pid from BASH
VAL_REAL=$!

# Wait for it to finish
wait $VAL_REAL

# Get its output
VAL_SNOOPY=`cat datasource_pid.out.tmp`

# Do a cleanup
rm -f datasource_pid.out.tmp



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
