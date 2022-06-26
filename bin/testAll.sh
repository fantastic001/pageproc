#!/bin/bash 

SAMPLE_CONFIG_DIR=$1
THIS_DIR=$(readlink -f $(dirname $0))
REPORTS_DIR=$2

mkdir -p $REPORTS_DIR

for config in $SAMPLE_CONFIG_DIR/*; do 
    REPORT_NAME=$(basename -s .config $config)
    REPORT=$REPORTS_DIR/$REPORT_NAME.txt 
    $THIS_DIR/test.sh $config > $REPORT
done 

