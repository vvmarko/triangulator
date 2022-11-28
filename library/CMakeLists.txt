add_library(TriangulatorLib STATIC)

# List of our source files.
list(APPEND TRIANGULATOR_SRCS
    implementation/color.cpp
    implementation/input_and_output.cpp
    implementation/ksimplex.cpp
    implementation/pachner.cpp
    implementation/seed.cpp
    implementation/simpcomp.cpp
    )

# List of directories to include header files from.
list(APPEND TRIANGULATOR_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    )

target_sources(TriangulatorLib PUBLIC ${TRIANGULATOR_SRCS})
target_include_directories(TriangulatorLib PUBLIC
                            ${TRIANGULATOR_HDRS})

# Setting compiler flags and connecting to RapidXML.
target_link_libraries(TriangulatorLib compiler_flags)
target_link_libraries(TriangulatorLib RapidXML)
