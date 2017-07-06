cmake_minimum_required(VERSION 2.8)
project(asyncnet_log)
set(SRC_PATH ${CMAKE_SOURCE_DIR}/)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
SET(SRC_LIST
${SRC_PATH}sinks.cc
${SRC_PATH}sinks.h
${SRC_PATH}log_stream.cc
${SRC_PATH}log_stream.h
${SRC_PATH}logging.cc
${SRC_PATH}logging.h
)
add_library(asyncnet_log STATIC ${SRC_LIST})
add_definitions("-g -DDEBUG -Wall -MMD -funroll-loops -std=c++11")
install(TARGETS asyncnet_log DESTINATION lib)
