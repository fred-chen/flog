#include <exception>

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
