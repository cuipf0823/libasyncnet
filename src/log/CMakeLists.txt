cmake_minimum_required(VERSION 2.8)
project(asyncnet_log)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
find_path(BASE_INCLUDE_DIR  mutex.h  ../base  DOC "the directory where mutex header exists")
set(SRC_PATH ${CMAKE_SOURCE_DIR}/)
set(BASE_PATH ${BASE_INCLUDE_DIR}/)
message(${SRC_PATH})
message(${BASE_INCLUDE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
set(SRC_LIST
${SRC_PATH}sinks.cc
${SRC_PATH}sinks.h
${SRC_PATH}log_stream.cc
${SRC_PATH}log_stream.h
${SRC_PATH}logging.cc
${SRC_PATH}logging.h
${SRC_PATH}file_util.h
${SRC_PATH}file_util.cc
${SRC_PATH}async_logging.h
${SRC_PATH}async_logging.cc
${SRC_PATH}dump_sinks.h
${SRC_PATH}dump_sinks.cc
${BASE_PATH}mutex.h
${BASE_PATH}mutex.cc
${BASE_PATH}condition.h
${BASE_PATH}condition.cc
)
add_library(asyncnet_log STATIC ${SRC_LIST})
add_definitions("-g -DDEBUG -Wall -MMD -funroll-loops -std=c++11")
install(TARGETS asyncnet_log DESTINATION lib)
