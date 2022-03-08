#include "gtest/gtest.h"
#include "flog.hpp"

using namespace FLOG;

TEST(testFlog, constants) {
    EXPECT_STREQ("INFO"  , SERVERITIES_STR[SEVERITIES::INFO].c_str());
    EXPECT_STREQ("WARN"  , SERVERITIES_STR[WARN].c_str());
    EXPECT_STREQ("ERROR" , SERVERITIES_STR[ERROR].c_str());
    EXPECT_STREQ(PROJECT_NAME "_log", g_flog_defaults.log_dir.c_str());
    EXPECT_STREQ(PROJECT_NAME ".err", g_flog_defaults.err_filename.c_str());
    EXPECT_STREQ(PROJECT_NAME ".log", g_flog_defaults.log_filename.c_str());
}

TEST(testFlog, tee_logger) {
    tee_logger logger(std::cout, std::cerr);
    logger << "Hello FOUT" << std::endl;
    logger << "ERROR FOUT" << std::endl;
}

TEST(testFlog, getLogger) {
    unlink(g_flog_settings.log_dir.c_str());
    tee_logger logger_err = getLogger(INFO);
    tee_logger logger_log = getLogger(ERROR);
    logger_log << "Hello FOUT" << std::endl;
    logger_err << "ERROR FOUT" << std::endl;
}

TEST(testFlog, LOG) {
    LOG(INFO) << "INFO FOUT" << std::endl;
    LOG(WARN) << "WARN FOUT" << std::endl;
    LOG(ERROR) << "ERROR FOUT" << std::endl;
}