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

enum SERVERITIES {
    log_level_list
};

#undef  level_element
#define level_element(X) #X,
std::vector<string> SERVERITIES_STR {
    log_level_list
};

struct FLOG_SETTINGS
{
    string log_dir      { PROJECT_NAME "_log" };
    string err_filename { PROJECT_NAME ".err" };
    string log_filename { PROJECT_NAME ".log" };
} g_flog_defaults, g_flog_settings;


bool file_exists ( const char * path ) {
    struct stat st;
    if ( stat(path, &st) == -1 ) {
        return false;
    }
    return true;
}

/**
 * @brief an 'ostream-like' object, that behaves like 'tee' command
 *        which prints the inputs to both screen and a file.
 * 
 */
class tee_logger {
    enum OUTPUTTYPE { MAIN_OUTPUT, SECOND_OUTPUT, BOTH_OUTPUT };
    private:
        OUTPUTTYPE     _output_type;    // whether to print to console
        std::ostream   _output;         // the main output object
        std::ostream   _output2;        // the 2nd output object
    public:
        tee_logger() : 
            _output(std::cout.rdbuf()),
            _output2(std::cerr.rdbuf()),
            _output_type(BOTH_OUTPUT) {}
        
        tee_logger( std::ostream & output, std::ostream & output2, 
                    OUTPUTTYPE type = BOTH_OUTPUT ) :
            _output_type(type),
            _output(output.rdbuf()),
            _output2(output2.rdbuf()) {}

        tee_logger( const tee_logger & rhs ) : tee_logger() {
            // _output_type = rhs._output_type;
            // _output.rdbuf(rhs._output.rdbuf());
            // _output2.rdbuf(rhs._output2.rdbuf());
        }
        
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

        void set_output_type ( OUTPUTTYPE type ) {
            _output_type = type;
        }

        std::ostream & getoutput() {
            return _output;
        }
        std::ostream & getoutput2() {
            return _output2;
        }
};

// time_t tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
// _errfile << std::put_time(std::localtime(&tm), "%Y-%m-%d %X");

/* SETTINGS */
void set_log_dir ( const string & path ) { 
    g_flog_settings.log_dir = path; 
}
void set_log_filename ( const string & name ) { 
    g_flog_settings.log_filename = name; 
}
void set_err_filename ( const string & name ) { 
    g_flog_settings.err_filename = name;
}

static tee_logger _flog_log; // the global log object
static tee_logger _flog_err; // the global err log object
static tee_logger getLogger( int severity ) {
    string path;
    std::filebuf * f;
    if( !file_exists(g_flog_settings.log_dir.c_str()) ) {
        ::mkdir(g_flog_settings.log_dir.c_str(), 0755);
    }
    if( severity == INFO || severity == WARN  ) {
        path = g_flog_settings.log_dir + "/" + g_flog_settings.log_filename;
        f    = static_cast<std::filebuf *>(_flog_log.getoutput().rdbuf());
    }
    else {
        path = g_flog_settings.log_dir + "/" + g_flog_settings.err_filename;
        f    = static_cast<std::filebuf *>(_flog_err.getoutput().rdbuf());
    }

    if( ! f->is_open() ) {
        f->open( path, std::ios::out|std::ios::app );
    }

    return severity == ERROR ? _flog_err : _flog_log;
}

#define LOG(SEVERITY) getLogger(SEVERITY)

} // namespace FLOG
