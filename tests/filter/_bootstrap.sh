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
export SNOOPY_TEST_BIN_PREFIX=`dirname $0`/../../src/bin/snoopy-test
export SNOOPY_TEST_DATASOURCE="${SNOOPY_TEST_BIN_PREFIX}-datasource"
export SNOOPY_TEST_FILTER="${SNOOPY_TEST_BIN_PREFIX}-filter"
