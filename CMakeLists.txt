cmake_minimum_required(VERSION 3.8)

if(NOT DEFINED PROJECT_NAME)
    project(flog CXX)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories(googletest/gtest/include)
add_subdirectory(googletest)

include(CTest)
enable_testing()

add_subdirectory(src)
