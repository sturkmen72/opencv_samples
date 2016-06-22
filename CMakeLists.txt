cmake_minimum_required(VERSION 2.8)

PROJECT (opencv_samples)

find_package(OpenCV  REQUIRED)
find_package(dlib QUIET)

macro(add_example name)
   add_executable(${name} ${name}.cpp)
   target_link_libraries(${name} ${OpenCV_LIBS} )
endmacro()

add_example(171913)
if (OpenCV_FOUND)
	include_directories( ${OpenCV_INCLUDE_DIRS} )
	include_directories( ${OpenCV_INCLUDE_DIRS} )
else (OpenCV_FOUND)
message("PB->OPENCV = ${OpenCV_INCLUDE_DIRS}")
endif (OpenCV_FOUND)