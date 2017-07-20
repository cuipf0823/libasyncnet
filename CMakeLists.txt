cmake_minimum_required(VERSION 3.6)
project(libasyncnet)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    src/poller/tests/epollpoller_test.h
    src/poller/tests/pollpoller_test.h
    src/poller/common.h
    src/poller/epollpoller.h
    src/poller/pollpoller.h
    libasyncnet.cc)

add_executable(libasyncnet ${SOURCE_FILES})
