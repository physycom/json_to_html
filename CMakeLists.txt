cmake_minimum_required (VERSION 3.1)
project (json_to_html)
set (CMAKE_CXX_STANDARD 11)

if (EXISTS "$ENV{WORKSPACE}/sysconfig/cmake/physycom_config.cmake")
   message(STATUS "Enabling physycom settings")
   include("$ENV{WORKSPACE}/sysconfig/cmake/physycom_config.cmake")
else()
   message(STATUS "Unable to find physycom settings file")
endif()

set(JSONCONS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/jsoncons/include)
set(UTILS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/utils)

include_directories(${UTILS_INCLUDE_DIR} ${JSONCONS_INCLUDE_DIR})

add_executable(json_to_html ${CMAKE_SOURCE_DIR}/src/json_to_html.cpp)
