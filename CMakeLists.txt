cmake_minimum_required(VERSION 3.6)
project(libasyncnet)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    module/poller/tests/epollpoller_test.h
    module/poller/tests/pollpoller_test.h
    module/poller/common.h
    module/poller/epollpoller.h
    module/poller/pollpoller.h
    libasyncnet.cc)

add_executable(libasyncnet ${SOURCE_FILES})