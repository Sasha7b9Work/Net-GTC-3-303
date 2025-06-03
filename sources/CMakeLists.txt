cmake_minimum_required(VERSION 3.10)

project (GTC-3)

set (TARGET_NAME GTC-3)

set(CMAKE_UNITY_BUILD ON)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(GTC-3)
