cmake_minimum_required (VERSION 2.6)

project (pageproc)

enable_testing()

find_package(OpenCV)
link_libraries(poppler ${OpenCV_LIBS})
include_directories(src/)

add_executable(pageproc
	src/_matpart.cpp
	src/debug.cpp
	src/segmentators/segmentation.cpp
	src/img_debug.cpp
	src/matpart.cpp
	src/classificators/recognition.cpp
	src/element.cpp

)

