cmake_minimum_required(VERSION 3.8)

execute_process(
    COMMAND uname "-s"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE CMAKE_OS_TYPE
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include_directories(googletest/gtest/include)
add_subdirectory(googletest EXCLUDE_FROM_ALL)

project(flog VERSION 0.0.1 LANGUAGES CXX)

include(CTest)
enable_testing()

add_subdirectory(src)

