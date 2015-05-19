#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data from Snoopy
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE username`
VAL_REAL=`id -a | grep -Eo 'uid=[0-9]+[(][^)]+[)]' | grep -Eo '[(][^)]+[)]' | grep -Eo '[^()]+'`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
