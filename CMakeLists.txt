cmake_minimum_required (VERSION 3.1)
project (json_to_html)
set (CMAKE_CXX_STANDARD 11)

set(JSONCONS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/jsoncons/src)
if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${JSONCONS_INCLUDE_DIR})

add_executable(json_to_html ${CMAKE_SOURCE_DIR}/src/json_to_html.cpp)
install (TARGETS json_to_html DESTINATION ${CMAKE_SOURCE_DIR}/bin)
