project (mainProject)
cmake_minimum_required (VERSION 2.8.11)

find_package( OpenCV REQUIRED )


link_directories(/usr/local/lib)
link_directories(${CMAKE_CURRENT_BINARY_DIR})


set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)

set (CMAKE_EXE_LINKER_FLAGS)
set (CMAKE_MODULE_LINKER_FLAGS)
set (CMAKE_SHARED_LINKER_FLAGS)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

add_executable	(mainProject main.cpp mySerial.h mySerial.cpp updateServos.h Tracker.h )

target_link_libraries(mainProject ${OpenCV_LIBS} )

