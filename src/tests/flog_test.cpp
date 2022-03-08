#include "gtest/gtest.h"
#include "flog.hpp"

using namespace FLOG;

void clear() {
    string path = g_flog_settings.log_dir + "/" + 
                  g_flog_settings.log_filename;
    unlink(path.c_str());
    path = g_flog_settings.log_dir + "/" + 
                  g_flog_settings.err_filename;
    unlink(path.c_str());
    unlink(g_flog_settings.log_dir.c_str());
}

TEST(testFlog, constants) {
    clear();
    EXPECT_STREQ("INFO"  , SERVERITIES_STR[SEVERITIES::INFO].c_str());
    EXPECT_STREQ("WARN"  , SERVERITIES_STR[WARN].c_str());
    EXPECT_STREQ("ERROR" , SERVERITIES_STR[ERROR].c_str());
    EXPECT_STREQ(PROJECT_NAME "_log", g_flog_defaults.log_dir.c_str());
    EXPECT_STREQ(PROJECT_NAME ".err", g_flog_defaults.err_filename.c_str());
    EXPECT_STREQ(PROJECT_NAME ".log", g_flog_defaults.log_filename.c_str());
}

TEST(testFlog, tee_logger) {
    tee_logger logger(std::cout, std::cerr);
    logger << "Hello tee_logger" << std::endl;
    logger << "ERROR tee_logger" << std::endl;
}

TEST(testFlog, getLogger) {
    tee_logger logger_err = getLogger(ERROR);
    tee_logger logger_log = getLogger(INFO);
    logger_log << "Hello getLogger" << std::endl;
    logger_err << "ERROR getLogger" << std::endl;
}

TEST(testFlog, LOG) {
    LOG(INFO)  << "INFO LOG"  << std::endl;
    LOG(WARN)  << "WARN LOG"  << std::endl;
    LOG(ERROR) << "ERROR LOG" << std::endl;
}