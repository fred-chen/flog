#include "gtest/gtest.h"
#include "flog.hpp"

using namespace std;

TEST(testFlog, example) {
    /**
     * tweak your settings before use
     * 
     */
    FLOG::set_log_dir("/tmp/mylog");     // default "./<projectname>_log"
    FLOG::set_log_filename("mylog.log"); // default "<projectname>.log"
    FLOG::set_err_filename("mylog.err"); // default "<projectname>.err"  

    /**
     *  default usage: use LOG() funcion directly
     * 
     */
    LOG(FLOG::SEVERITIES::INFO) << "INFO FLOG (example)!!!" << endl;
    // or
    using namespace FLOG;
    LOG(SEVERITIES::WARN) << "WARN FLOG (example)!!!" << endl;
    // or
    using namespace FLOG;
    LOG(ERROR) << "ERROR FLOG (example)!!!" << endl;

    /* 
     * after above, check the INFO and WARN content in log file 
     * /tmp/<projectname>_log/flog.log  
     * and check the ERROR content in error log file 
     * /tmp/<projectname>/flog.err
     */
    
    /**
     *    by default, log content will be written to log file
     *    and printed on screen.
     *    this can be changed with set_log_devices()
     *    examples:
     *      set_log_devices( LOGFILE )     log file only
     *      set_log_devices( SCREEN  )     screen only
     *      set_log_devices( BOTH_OUTPUT ) both
     */
    FLOG::set_log_devices( LOGFILE );     // write to log file only
    LOG(ERROR) << "ERROR After reopen with LOGFILE (by example)" << endl;
    LOG(INFO) << "INFO After reopen with LOGFILE (by example)" << endl;
    /* 
     * after above, check the log file /tmp/<projectname>_log/flog.log  
     * and /tmp/<projectname>/flog.err 
     */

    FLOG::set_log_devices( SCREEN );     // write to screen only
    LOG(ERROR) << "ERROR After reopen with SCREEN (by example)" << endl;
    LOG(INFO) << "INFO After reopen with SCREEN (by example)" << endl;
    /* 
     * above will only be seen on screen 
     */
}