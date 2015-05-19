#!/bin/bash



### Configure shell
#
set -e
set -u



### Include main _bootstrap.sh
#
. ../_bootstrap.sh



### Paths
#
export SNOOPY_TEST_BIN_PREFIX=`dirname $0`/../bin/snoopy-test
export SNOOPY_TEST_OUTPUT="${SNOOPY_TEST_BIN_PREFIX}-output"
