/**
 * @file flog.hpp
 * @author Fred Chen (fred.chen@live.com)
 * @brief  a simple C++ style log module
 * @version 0.1
 * @date 2022-03-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <string>
#include <vector>
#include "flog_config.hpp"

using std::string;

#undef  level_element
#define level_element(X) X,
#define log_level_list          \
        level_element(INFO)     \
        level_element(WARN)     \
        level_element(ERROR)

enum log_levels {
    log_level_list
};

#undef  level_element
#define level_element(X) #X,
std::vector<string> log_level_list_str {
    log_level_list
};

struct FLOG_DEFAULTS
{
    const string default_log_dir      { PROJECT_NAME "_log"   };
    const string default_err_filename { PROJECT_NAME ".error" };
    const string default_log_filename { PROJECT_NAME ".log"   };
} g_flog_defaults;

