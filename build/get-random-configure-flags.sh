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
ALL_CONFIGURE_OPTS="$OTHERS $DATASOURCES $FILTERS $OUTPUTS"

# Make associative array for this
declare -A ALL_CONFIG_OPTS_ASSOC
for OPT in $ALL_CONFIGURE_OPTS; do
    ALL_CONFIG_OPTS_ASSOC[$OPT]="unspecified"
done



### Select random set of options
#
for OPT in "${!ALL_CONFIG_OPTS_ASSOC[@]}"; do
    RN=$[ RANDOM % 3 ]
    case $RN in
        0)
            ALL_CONFIG_OPTS_ASSOC[$OPT]="enable"
            ;;
        1)
            ALL_CONFIG_OPTS_ASSOC[$OPT]="disable"
            ;;
        2)
            ALL_CONFIG_OPTS_ASSOC[$OPT]="unspecified"
            ;;
    esac
done



### Adjust selections
#
# Thread safety needs to be enabled for this datasource
#
if [ "${ALL_CONFIG_OPTS_ASSOC["datasource-snoopy_threads"]}" == "enable" ]; then
    ALL_CONFIG_OPTS_ASSOC["thread-safety"]="enable"
fi
if [[ "${ALL_CONFIG_OPTS_ASSOC["everything"]}" == "enable" ]] || [[ "${ALL_CONFIG_OPTS_ASSOC["all-datasources"]}" == "enable" ]]; then
    if [ "${ALL_CONFIG_OPTS_ASSOC["datasource-snoopy_threads"]}" != "disable" ]; then
        ALL_CONFIG_OPTS_ASSOC["thread-safety"]="enable"
    fi
fi
if [[ "${ALL_CONFIG_OPTS_ASSOC["everything"]}" == "unspecified" ]] && [[ "${ALL_CONFIG_OPTS_ASSOC["all-datasources"]}" == "unspecified" ]]; then
    if [ "${ALL_CONFIG_OPTS_ASSOC["datasource-snoopy_threads"]}" != "disable" ]; then
        ALL_CONFIG_OPTS_ASSOC["thread-safety"]="enable"
    fi
fi



### Build output
#
SELECTED_OPTS=""
for OPT in "${!ALL_CONFIG_OPTS_ASSOC[@]}"; do
    case ${ALL_CONFIG_OPTS_ASSOC[$OPT]} in
        enable)
            SELECTED_OPTS="$SELECTED_OPTS --enable-$OPT"
            ;;
        disable)
            SELECTED_OPTS="$SELECTED_OPTS --disable-$OPT"
            ;;
    esac
done



### Final output
#
echo $SELECTED_OPTS
