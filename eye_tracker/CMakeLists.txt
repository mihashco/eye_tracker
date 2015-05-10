cmake_minimum_required(VERSION 2.8.12)
project(main)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SRCs
	"*.h"
	"*.cpp"
)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

add_executable(main ${SRCs})
target_link_libraries(main ${OpenCV_LIBS})