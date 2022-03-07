include_directories(include)

include(GNUInstallDirs)

add_subdirectory(tests)
add_subdirectory(include)

set(CPACK_GENERATOR "RPM")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL headers)
include(CPack)