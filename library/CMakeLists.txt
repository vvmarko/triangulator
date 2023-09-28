add_library(Triangulator)

# List of our source files.
list(APPEND TRIANGULATOR_SRCS
    src/color.cpp
    src/input_and_output.cpp
    src/ksimplex.cpp
    src/pachner.cpp
    src/seed.cpp
    src/simpcomp.cpp
    src/global-vars.cpp
  )

# List of directories to include header files from.
list(APPEND TRIANGULATOR_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

	target_sources(Triangulator PRIVATE ${TRIANGULATOR_SRCS})
target_include_directories(Triangulator PUBLIC
                            ${TRIANGULATOR_HDRS})

# Setting compiler flags and connecting to RapidXML.
target_link_libraries(Triangulator compiler_flags)
target_link_libraries(Triangulator RapidXML)

install(TARGETS Triangulator
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/precompiled/triangulator
        )
