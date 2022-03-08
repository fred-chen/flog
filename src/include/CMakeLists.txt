set(
WARNING_TEXT_DONTMODIFY 
"      DON'T MODIFY THIS FILE!!!
---------------------------------------
This is an auto-generated file by cmake.
Any modification will be overwritten.
'flog.hpp.in' is the source file."
)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/flog.hpp.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/flog.hpp
)

install(
    FILES flog.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/flog
)
