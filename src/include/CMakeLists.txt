CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/flog_config.hpp)

install(
    FILES flog.hpp flog_config.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/flog
    COMPONENT headers
)
