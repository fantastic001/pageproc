#!/bin/sh


g++ test_${1}.cpp -o test_$1 -lboost_unit_test_framework -I../src/ -lopencv_core -lopencv_highgui -I/usr/include/opencv2

./test_${1}
