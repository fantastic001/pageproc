#!/bin/bash 


if [ -z "$1" ]; then 
    MODEL="neural_network"
    PREPROCESSOR="simple"
else 
    . $1
fi 

THIS_DIR=$(readlink -f $(dirname $0))
INVOCATION_ID=$(date +%s)

WORKDIR=$THIS_DIR/work-$INVOCATION_ID

mkdir $WORKDIR
SAMPLE_PDF="/home/stefan/data/Documents/Library/Papers/Computer Science/Computer graphics/J. Olsen - Realtime procedural terrain generation (2004).pdf"
BASE_SEGMENTS=$WORKDIR/data 
TRAINING_DIR=$WORKDIR/training-data 
TEST_DIR=$WORKDIR/test-data 
LABEL_FILE=$WORKDIR/labels.csv
MODEL_FILE=$WORKDIR/model.json
TRAINING_SIZE=3


pushd $THIS_DIR/..


python -m pdfproc --segment "$SAMPLE_PDF"  --output $BASE_SEGMENTS \
    --vertical-whitespace-tolerance 20 \
    --horizontal-whitespace-tolerance 20 \
    --depth 2 \
    --leaf-only


cp data.csv $LABEL_FILE

python -m segment_classify assign --csv $LABEL_FILE --data $BASE_SEGMENTS

mkdir -p $TEST_DIR
mkdir -p $TRAINING_DIR

echo "Creating training set..."
PAGE_NUM=$(ls $BASE_SEGMENTS | wc -l)


while [ $(ls $TRAINING_DIR | wc -l) -lt $TRAINING_SIZE ]; do 
    index=$(($RANDOM % $PAGE_NUM))
    if [ ! -d $TRAINING_DIR/page_$index ]; then 
        echo "$index will be used in training"
        cp -r $BASE_SEGMENTS/page_$index $TRAINING_DIR/
    fi
done

# Create test directory 
echo "Creating test directory"
for f in $BASE_SEGMENTS/*; do 
    page=$(basename $f)
    if [ ! -d $TRAINING_DIR/$page ]; then 
        echo "$page will be used for testing"
        cp -r $BASE_SEGMENTS/$page $TEST_DIR/
    fi 
done 


echo "Training model"
python -m segment_classify train --data-dir $TRAINING_DIR --output $MODEL_FILE --classifier $MODEL --preprocessor $PREPROCESSOR

echo "Testing model..."
python -m segment_classify classify --data-dir $TEST_DIR --model $MODEL_FILE

python -m segment_classify score --data-dir $TEST_DIR

if [ "$DEBUG" != "1" ]; then 
    rm -rf $WORKDIR
fi 
popd 