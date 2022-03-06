add_executable(flog_test flog_test.cpp)
target_link_libraries(flog_test gtest gtest_main)
add_test(NAME flog_test COMMAND flog_test)