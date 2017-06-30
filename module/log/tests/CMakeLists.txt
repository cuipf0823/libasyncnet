cmake_minimum_required(VERSION 3.6)
project(asyncnet_log)
set(SRC_PATH ${CMAKE_SOURCE_DIR}/)
SET(SRC_LIST
${SRC_PATH}log_stream_test.cc
)
add_executable(log_test ${SRC_LIST}})
add_definitions("-g -DDEBUG -Wall -MMD -funroll-loops -std=c++11")
