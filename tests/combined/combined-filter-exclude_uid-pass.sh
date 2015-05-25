#!/bin/bash



### Configure shell
#
set -e
set -u



### Set test configuration
#
CUR_TIMESTAMP=`date +%s`
MY_UID=`id -a | grep -Eo 'uid=[0-9]+' | grep -Eo '[0-9]+'`
MY_UID_PLUS_1=`expr $MY_UID + 1`
ARG_1="ls -la /"
ARG_2="$CUR_TIMESTAMP %{cmdline}"
ARG_3="exclude_uid:$MY_UID_PLUS_1"
ARG_4="1"
ARG_5="$CUR_TIMESTAMP ls -la /"



### Call/source common combined test implementation
#
. `dirname $BASH_SOURCE`/combined-generic.sh
