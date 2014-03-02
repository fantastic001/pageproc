#!/bin/bash

g++ -o test *.cpp ./../src/*.cpp -I/usr/include/opencv2/ -I./../src/ -lopencv_core -lopencv_highgui
if [ $? = 0 ]; then 
	./test 
fi
