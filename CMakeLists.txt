cmake_minimum_required(VERSION 2.8) 

PROJECT (noteScanner)
find_package(OpenCV REQUIRED )

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/copies")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET( NAME_SRC
    src/main.cpp
)
SET( NAME_HEADERS       
     include/myLibrary.h
)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
link_directories(${OpenCV_LIB_DIR})
add_executable( noteScanner ${NAME_SRC} ${NAME_HEADERS} )
target_link_libraries( noteScanner ${OpenCV_LIBS} )