#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Default config
#
ITERATIONS="100"
THREADS="200"



### Override with CLI args
#
if [ ! -z ${1:-} ]; then
    ITERATIONS="$1"
fi
if [ ! -z ${2:-} ]; then
    THREADS="$2"
fi



### Run the test
#
echo "Running $ITERATIONS iterations with $THREADS threads in each:"
for i in `seq 1 $ITERATIONS`; do
    echo -n "    Iteration #$i... "
    $SNOOPY_TEST_THREADS   $THREADS > /dev/null
    RES=$?
    if [ "$RES" == "0" ]; then
        echo "done, exit status = $?"
    else
        echo
        echo "        ERROR: Exit status = $?"
    fi
done



### Evaluate
#
snoopy_testResult_pass "No segmentation faults occured. Iterations: $ITERATIONS. Threads-per-iteration: $THREADS."
