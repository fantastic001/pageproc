cmake_minimum_required (VERSION 2.6)

project (pageproc)

enable_testing()

find_package( OpenCV REQUIRED )
link_libraries(${OpenCV_LIBS})

SET(core_source_files 
	src/debug.cpp
	src/element.cpp
	src/img_debug.cpp
	src/matpart.cpp
	src/recognition.cpp
	src/segmentation.cpp	
)

include_directories(
	
)

add_library(pageproc SHARED 
	${core_source_files}
)

add_executable(unittest 
	${core_source_files}
	test/mytest.cpp
)
target_link_libraries(unittest boost_unit_test_framework)
add_test(NAME unittest COMMAND unittest)

