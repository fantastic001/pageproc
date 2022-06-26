#!/bin/bash 

SAMPLE_CONFIG_DIR=$1
THIS_DIR=$(readlink -f $(dirname $0))
REPORTS_DIR=$2

MAX_REPORTS_PER_CONF=10
mkdir -p $REPORTS_DIR

for config in $SAMPLE_CONFIG_DIR/*; do 
    REPORT_NAME=$(basename -s .config $config)
    REPORT=$REPORTS_DIR/$REPORT_NAME
    for ((i=0; i<$MAX_REPORTS_PER_CONF; i++)); do 
        mkdir -p $REPORT
        $THIS_DIR/test.sh $config > $REPORT/$i.txt
    done 
done 

