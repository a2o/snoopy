#!/bin/bash



### Configure shell
#
set -e
set -u



### Include main _bootstrap.sh
#
. `dirname $BASH_SOURCE`/../_bootstrap.sh



### This dir-specific settings
#
# Path to the freshly built `snoopy` CLI tool
export SNOOPY_CLI="$SNOOPY_CUR_TEST_WORKDIR/../../src/cli/snoopy"
# This one changes the behaviour of `snoopy ...` CLI tool, to look for `libsnoopy.so`
# in the specified location instead of the configured one.
# For now, this does not change what `snoopy enable` puts into the `ld.so.preload` file.
export SNOOPY_TEST_LIBSNOOPY_SO_PATH="$SNOOPY_CUR_TEST_WORKDIR/../../src/.libs/libsnoopy.so"
