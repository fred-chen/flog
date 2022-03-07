/**
 * @file flog.hpp
 * @author Fred Chen (fred.chen@live.com)
 * @brief  a simple C++ style log module
 * @date 2022-03-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "flog_config.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include "singleton.hpp"

namespace FLOG {

using std::string;

#undef  level_element
#define level_element(X) X,
#define log_level_list          \
        level_element(INFO)     \
        level_element(WARN)     \
        level_element(ERROR)

enum flog_severities {
    log_level_list
};

#undef  level_element
#define level_element(X) #X,
std::vector<string> log_levels_str {
    log_level_list
};

struct FLOG_SETTINGS
{
    string log_dir      { PROJECT_NAME "_log" };
    string err_filename { PROJECT_NAME ".err" };
    string log_filename { PROJECT_NAME ".log" };
} g_flog_defaults;


bool file_exists ( const char * path ) {
    struct stat st;
    if ( stat(path, &st) == -1 ) {
        return false;
    }
    return true;
}

/**
 * @brief a 'ostream-like' object, that behaves like 'tee' command
 *        which prints the inputs to both screen and a file
 * 
 */
class tee_logger {
    using SEVERITY = flog_severities;
    enum OUTPUTTYPE { MAIN_OUTPUT, SECOND_OUTPUT, BOTH_OUTPUT };
    private:
        OUTPUTTYPE     _output_type;    // whether to print to console
        SEVERITY       _sev;            // the log level
        std::ostream   _output;         // the main output object
        std::ostream   _output2;        // the 2nd output object
    public:
        tee_logger() : 
            _output(std::cout.rdbuf()),
            _output2(std::cerr.rdbuf()),
            _output_type(BOTH_OUTPUT) {}
        
        tee_logger( std::ostream & output, std::ostream & output2 ) : 
            _output_type(BOTH_OUTPUT),
            _output(output.rdbuf()),
            _output2(output2.rdbuf()) {}
        
        /* below simulation funcitons of a real ostream */
        
        /**
         * @brief simulates ostream operators:
         *        ostream& operator<< (bool val);
         *        ostream& operator<< (short val);
         *        ostream& operator<< (unsigned short val);
         *        ostream& operator<< (int val);
         *        ostream& operator<< (unsigned int val);
         *        ostream& operator<< (long val);
         *        ostream& operator<< (unsigned long val);
         *        ostream& operator<< (float val);
         *        ostream& operator<< (double val);
         *        ostream& operator<< (long double val);
         *        ostream& operator<< (void* val);
         * 
         * @tparam T 
         * @param val 
         * @return ostream& 
         */
        template <typename T>
        tee_logger & operator<< ( T val ) {
            if( _output_type == MAIN_OUTPUT || _output_type == BOTH_OUTPUT ) {
                _output << val;
            }
            if( _output_type == SECOND_OUTPUT || _output_type == BOTH_OUTPUT ) {
                _output2 << val;
            }
            return (*this);
        }

        /**
         * @brief simulates ostream operators:
         *        ostream& operator<< (ostream& (*pf)(ostream&));
         *        ostream& operator<< (ios& (*pf)(ios&));
         *        ostream& operator<< (ios_base& (*pf)(ios_base&));
         * @param pf 
         * @return ostream& 
         */
        tee_logger & operator<< (std::ostream& (*pf)(std::ostream&)) {
            if( _output_type == MAIN_OUTPUT || _output_type == BOTH_OUTPUT ) {
                pf(_output2);
            }
            if( _output_type == SECOND_OUTPUT || _output_type == BOTH_OUTPUT ) {
                pf(_output);
            }
            return (*this);
        }
        tee_logger & operator<< (std::ios& (*pf)(std::ios&)) {
            pf(_output2);
            pf(_output);
            return (*this);
        }
        tee_logger & operator<< (std::ios_base& (*pf)(std::ios_base&)) {
            pf(_output2);
            pf(_output);
            return (*this);
        }
};

class logger : public Singleton<logger> {
    using SEVERITY = flog_severities;
    enum OUTPUTTYPE { MAIN_OUTPUT, SECOND_OUTPUT, BOTH };
    private:
        struct         FLOG_SETTINGS _settings;
        OUTPUTTYPE     _output_type;    // a switch to control whether to print to console
        std::ofstream  _logfile;        // the log file
        std::ofstream  _errfile;        // the error log file
        SEVERITY       _sev;            // the log level
    private:
        void openlog() {
            if ( ! _logfile.is_open() ) {
                if ( !file_exists(_settings.log_dir.c_str()) ) {
                    ::mkdir(_settings.log_dir.c_str(), 0755);
                }
                std::string path = _settings.log_dir + _settings.err_filename;
                _logfile.open(path, std::ios_base::app);
            }
        }
        void openerr() {
            if ( ! _errfile.is_open() ) {
                if ( !file_exists(_settings.log_dir.c_str()) ) {
                    ::mkdir(_settings.log_dir.c_str(), 0755);
                }
                std::string path = _settings.log_dir + _settings.err_filename;
                _errfile.open(path, std::ios_base::app);
            }
        }
    public:
        logger() : _output_type(BOTH) {}

        template <class T>
        std::ostream & operator << (T val) {
            time_t tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            if ( _sev == INFO || _sev == WARN ) {
                openlog();
                return _logfile << std::put_time(std::localtime(&tm), "%Y-%m-%d %X");
            }
            else {
                // error log
                openerr();
                return _errfile << std::put_time(std::localtime(&tm), "%Y-%m-%d %X");
            }
        }

        std::ostream & operator << ( std::ostream & (*pfun)( std::ostream & ) )
        {
            if ( _sev == INFO || _sev == WARN ) {
                pfun(std::cout);
                return pfun(_logfile);
            }
            else {
                // error log
                pfun(std::cout);
                return pfun(_errfile);
            }
        }

        /* setters */
        void set_log_dir ( const string & path ) { 
            _settings.log_dir = path; 
        }
        void set_log_filename ( const string & name ) { 
            _settings.log_filename = name; 
        }
        void set_to_console ( OUTPUTTYPE output_type ) { 
            _output_type = output_type; 
        }
};

#define LOG(LEVEL) ( logger::instance() )

} // namespace FLOG
