#include "gtest/gtest.h"
#include "flog.hpp"

using namespace FLOG;

TEST(testFlog, constants) {
    EXPECT_STREQ("INFO"  , log_levels_str[flog_severities::INFO].c_str());
    EXPECT_STREQ("WARN"  , log_levels_str[WARN].c_str());
    EXPECT_STREQ("ERROR" , log_levels_str[ERROR].c_str());
    EXPECT_STREQ(PROJECT_NAME "_log", g_flog_defaults.log_dir.c_str());
    EXPECT_STREQ(PROJECT_NAME ".err", g_flog_defaults.err_filename.c_str());
    EXPECT_STREQ(PROJECT_NAME ".log", g_flog_defaults.log_filename.c_str());
}

TEST(testFlog, tee_logger) {
    tee_logger logger(std::cout, std::cerr);
    logger << "Hello FOUT" << std::endl;
    logger << "ERROR FOUT" << std::endl;
}