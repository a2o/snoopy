#!/bin/bash



### Configure shell
#
set -e
set -u
MYDIR=`dirname $0`



### Collect all ./configure settings
#
# Ignore the following switches:
#   --only-root (deprecated),
#   --enable-dev-tools (not needed),
#   --output-file (always needed)
#   --output-socket (always needed)
#
OTHERS="
everything
config-file
error-logging
thread-safety
all-datasources
all-outputs
filtering
all-filters
"
DATASOURCES=`cat $MYDIR/../configure.ac | grep SNOOPY_CONFIGURE_DATASOURCE_ | cut -d'[' -f2 | cut -d']' -f1 | sed -e 's/^/datasource-/'`
FILTERS=`cat     $MYDIR/../configure.ac | grep SNOOPY_CONFIGURE_FILTER_     | cut -d'[' -f2 | cut -d']' -f1 | sed -e 's/^/filter-/'`
OUTPUTS=`cat     $MYDIR/../configure.ac | grep SNOOPY_CONFIGURE_OUTPUT_     | cut -d'[' -f2 | cut -d']' -f1 | sed -e 's/^/output-/' | grep -Ev '^output-(file|socket)$'`

ALL_CONFIGURE_OPTIONS="$OTHERS $DATASOURCES $FILTERS $OUTPUTS"



### Select random set of options
#
SELECTED_OPTIONS=""
for OPT in $ALL_CONFIGURE_OPTIONS; do
    RN=$[ RANDOM % 3 ]
    case $RN in
        0)
            SELECTED_OPTIONS="$SELECTED_OPTIONS --enable-$OPT"
            ;;
        1)
            SELECTED_OPTIONS="$SELECTED_OPTIONS --disable-$OPT"
            ;;
        2)
            SELECTED_OPTIONS="$SELECTED_OPTIONS" # Unspecified
            ;;
    esac
done



### Final output
#
echo $SELECTED_OPTIONS
