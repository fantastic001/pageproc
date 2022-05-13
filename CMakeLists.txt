cmake_minimum_required (VERSION 3.0)

project (pageproc)

enable_testing()
find_package( OpenCV REQUIRED )

# TODO: There are some issues with this approach where CMake requests too many unnecessary dependencies
# find_package(VTK REQUIRED)
# link_libraries(${OpenCV_LIBS} ${CLI11_LIBS})

find_package(CLI11 REQUIRED)
link_libraries(opencv_core opencv_highgui opencv_imgproc ${CLI11_LIBS})


SET(core_source_files 
	src/debug.cpp
	src/element.cpp
	src/img_debug.cpp
	src/matpart.cpp
	src/recognition.cpp
	src/segmentation.cpp	
)

include_directories(
	src/
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


add_executable(pdfproc pdfproc.cpp)
target_link_libraries(pdfproc pageproc)

find_package(Python COMPONENTS Interpreter REQUIRED)

# detect virtualenv and set Pip args accordingly
if(DEFINED ENV{VIRTUAL_ENV} OR DEFINED ENV{CONDA_PREFIX})
  set(_pip_args)
else()
  set(_pip_args "--user")
endif()

set(PY_INSTALL "execute_process(COMMAND python/install.sh ${Python_EXECUTABLE})")
install(CODE ${PY_INSTALL})