#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get arguments
#
# If this file is sourced from other tests, get arguments from ARG_x variables.
# Otherwise use CLI args.
#
if [ "x$ARG_1" != "x" ]; then
    COMMAND="$ARG_1"
    MESSAGE_FORMAT="$ARG_2"
    FILTER_CHAIN="$ARG_3"
    EXPECTED_OUTPUT_LINECOUNT="$ARG_4"
    EXPECTED_OUTPUT_CONTENT="$ARG_5"
else
    COMMAND="$1"
    MESSAGE_FORMAT="$2"
    FILTER_CHAIN="$3"
    EXPECTED_OUTPUT_LINECOUNT="$4"
    EXPECTED_OUTPUT_CONTENT="$5"
fi

TMP_INI=$SNOOPY_CUR_TEST_FILENAME.$$.ini
TMP_OUT=$SNOOPY_CUR_TEST_FILENAME.$$.out
# Remove possibly stale files
rm -f $TMP_INI
rm -f $TMP_OUT
# Readlink fails if file does not exist
touch $TMP_INI
touch $TMP_OUT
# Make paths absolute
TMP_INI=`readlink -e $TMP_INI`
TMP_OUT=`readlink -e $TMP_OUT`

cat > $TMP_INI <<EOF
[snoopy]
message_format = "$MESSAGE_FORMAT"
filter_chain   = "$FILTER_CHAIN"
output         = "file:$TMP_OUT"
EOF



### Execute requested command
#
# Execute in subshell, to prevent output file polution
#
export SNOOPY_INI=`readlink -e $TMP_INI`
export LD_PRELOAD=$SNOOPY_LIBSNOOPY_TEST_SO_PATH
(
    echo "$COMMAND" | bash > /dev/null
)
unset LD_PRELOAD
unset SNOOPY_INI



### Compare output line count
#
OUTPUT_LINECOUNT=`cat $TMP_OUT | wc -l`
if [ "$OUTPUT_LINECOUNT" != "$EXPECTED_OUTPUT_LINECOUNT" ]; then
    snoopy_testResult_fail "Expected line count ($EXPECTED_OUTPUT_LINECOUNT) does not equal actual line count ($OUTPUT_LINECOUNT)."
fi
# If linecount is zero, there is no point comparing content
if [ "$OUTPUT_LINECOUNT" == "0" ]; then
    # Cleanup too
    rm -f $TMP_INI
    rm -f $TMP_OUT
    snoopy_testResult_pass
fi



### Compare content
#
OUTPUT_CONTENT=`cat $TMP_OUT`



### Cleanup
#
rm -f $TMP_INI
rm -f $TMP_OUT



### Evaluate
#
snoopy_test_compareValues "$OUTPUT_CONTENT" "$EXPECTED_OUTPUT_CONTENT"
