/**
 * @file flog.hpp
 * @author Fred Chen (fred.chen@live.com)
 * @brief  a simple C++ style log module
 * @date 2022-03-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define PROJECT_NAME "flog"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <exception>

namespace FLOG {


class flog_exception : public std::exception {
    int   _errno;
    std::string _errstr;
    std::string _whatmsg;
public:
    flog_exception(std::string msg) noexcept : 
        _errno(errno), _errstr("")
    {
        _whatmsg = msg + "(errno(" + 
                   std::to_string(errno) + "): " + 
                   ::strerror(errno) + ")";
    }
    const char* what() const noexcept {
        return _whatmsg.c_str();
    }
};

using std::string;

#undef  level_element
#define level_element(X) X,
#define log_level_list          \
        level_element(INFO)     \
        level_element(WARN)     \
        level_element(ERROR)

enum SEVERITIES {
    log_level_list
};

#undef  level_element
#define level_element(X) #X,
std::vector<string> SERVERITIES_STR {
    log_level_list
};

enum OUTPUTTYPE { MAIN_OUTPUT, SECOND_OUTPUT, BOTH_OUTPUT };

struct FLOG_SETTINGS
{
    string log_dir      { PROJECT_NAME "_log" };
    string err_filename { PROJECT_NAME ".err" };
    string log_filename { PROJECT_NAME ".log" };
    OUTPUTTYPE output_type { BOTH_OUTPUT };
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
    private:
        OUTPUTTYPE     _output_type;    // whether to print to console
        std::ostream   _output;         // the main output object
        std::ostream   _output2;        // the 2nd output object
        bool           _initialized;    // is it initialized
    public:
        tee_logger() : 
            _output(std::cout.rdbuf()),
            _output2(std::cerr.rdbuf()),
            _output_type(BOTH_OUTPUT),
            _initialized(false) {}
        
        tee_logger( std::ostream & output, std::ostream & output2, 
                    OUTPUTTYPE type = BOTH_OUTPUT ) :
            _output_type(type),
            _output(output.rdbuf()),
            _output2(output2.rdbuf()),
            _initialized(true) {}

        tee_logger( const tee_logger & rhs ) : tee_logger() {
            _output_type = rhs._output_type;
            _output.rdbuf(rhs._output.rdbuf());
            _output2.rdbuf(rhs._output2.rdbuf());
            _initialized = rhs._initialized;
        }

        void init( std::ostream & output, std::ostream & output2, 
                    OUTPUTTYPE type = BOTH_OUTPUT ) {
            _output_type = type;
            _output.rdbuf(output.rdbuf());
            _output2.rdbuf(output2.rdbuf());
            _initialized = true;
        }
        void init( std::streambuf * output_buf, std::streambuf * output2_buf, 
                    OUTPUTTYPE type = BOTH_OUTPUT ) {
            _output_type = type;
            _output.rdbuf(output_buf);
            _output2.rdbuf(output2_buf);
            _initialized = true;
        }
        void uninit() {
            _initialized = false;
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
                pf(_output);
            }
            if( _output_type == SECOND_OUTPUT || _output_type == BOTH_OUTPUT ) {
                pf(_output2);
            }
            return (*this);
        }
        tee_logger & operator<< (std::ios& (*pf)(std::ios&)) {
            if( _output_type == MAIN_OUTPUT || _output_type == BOTH_OUTPUT ) {
                pf(_output);
            }
            if( _output_type == SECOND_OUTPUT || _output_type == BOTH_OUTPUT ) {
                pf(_output2);
            }
            return (*this);
        }
        tee_logger & operator<< (std::ios_base& (*pf)(std::ios_base&)) {
            if( _output_type == MAIN_OUTPUT || _output_type == BOTH_OUTPUT ) {
                pf(_output);
            }
            if( _output_type == SECOND_OUTPUT || _output_type == BOTH_OUTPUT ) {
                pf(_output2);
            }
            return (*this);
        }

        bool operator== (tee_logger & rhs) {
            return ( (rhs.getoutput().rdbuf()  == _output.rdbuf()) && 
                     (rhs.getoutput2().rdbuf() == _output2.rdbuf()) );
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
        bool initialized() const {
            return _initialized;
        }
};


static tee_logger & getLogger( int severity ) {
    static std::filebuf   _fbuf_log;
    static std::filebuf   _fbuf_err;
    static tee_logger _flog_log; // the global log object
    static tee_logger _flog_err; // the global err log object
    string path;

    tee_logger&      logger = (severity == ERROR) ? _flog_err : _flog_log;
    std::filebuf&    fbuf   = (severity == ERROR) ? _fbuf_err : _fbuf_log;
    std::streambuf * sbuf   = (severity == ERROR) ? std::cerr.rdbuf()
                                                  : std::cout.rdbuf();

    if( logger.initialized() ) {
        // the logger can be used without initialization
        return logger;
    }

    if( !file_exists(g_flog_settings.log_dir.c_str()) ) {
        if( ::mkdir(g_flog_settings.log_dir.c_str(), 0755) == -1 ) {
            throw( flog_exception(std::string("failed to mkdir ") + 
                        g_flog_settings.log_dir) );
        }
    }
    if( severity == INFO || severity == WARN  ) {
        path = g_flog_settings.log_dir + "/" + g_flog_settings.log_filename;
    }
    else {
        path = g_flog_settings.log_dir + "/" + g_flog_settings.err_filename;
    }

    fbuf.open(path, std::ios::app|std::ios::out);
    logger.init( &fbuf, sbuf, g_flog_settings.output_type );

    return logger;
}


void reopen() { 
    getLogger(ERROR).uninit();
    getLogger(INFO).uninit();
}
/* SETTINGS */
void set_log_dir ( const string & path ) {
    g_flog_settings.log_dir = path;
    reopen();
}
void set_log_filename ( const string & name ) {
    g_flog_settings.log_filename = name;
    reopen();
}
void set_err_filename ( const string & name ) {
    g_flog_settings.err_filename = name;
    reopen();
}

#define LOGFILE OUTPUTTYPE::MAIN_OUTPUT
#define SCREEN  OUTPUTTYPE::SECOND_OUTPUT
void set_log_devices ( OUTPUTTYPE type ) {
    g_flog_settings.output_type = type;
    getLogger(ERROR).set_output_type(type);
    getLogger(INFO).set_output_type(type);
}


tee_logger & LOG(SEVERITIES severity) {
    time_t tm = std::chrono::system_clock::to_time_t(
                        std::chrono::system_clock::now());
    return getLogger(severity) << std::setw(5) << std::left
                << SERVERITIES_STR[severity] << " "
                << std::put_time(std::localtime(&tm), "%Y-%m-%d %X")
                << " ";
}


} // namespace FLOG
