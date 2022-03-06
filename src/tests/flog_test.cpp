#include "gtest/gtest.h"
#include "flog.hpp"

TEST(testFlog, constants) {
    EXPECT_STREQ("INFO", log_level_list_str[log_levels::INFO].c_str());
    EXPECT_STREQ("WARN", log_level_list_str[WARN].c_str());
    EXPECT_STREQ("ERROR", log_level_list_str[ERROR].c_str());
}
