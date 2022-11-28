add_library(RapidXML INTERFACE)

# This is a header-only library, so we just need to interface it.
target_include_directories(RapidXML INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})

# Setting compiler flags.
target_link_libraries(RapidXML INTERFACE compiler_flags)
