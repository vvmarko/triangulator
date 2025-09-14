# - Enforce an out-of-source builds before anything else
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  # The odd formatting of the string is needed to comply with CMake's display rules!
  message(FATAL_ERROR
"Triangulator in-source build detected.
 Please remove these files from ${CMAKE_CURRENT_BINARY_DIR}:
  \n  CMakeCache.txt
  CMakeFiles
 \n Once these files are removed, create a separate directory and run CMake from there")
endif()


# Setting minimum required version for CMake.
cmake_minimum_required(VERSION 3.15)

# Project name and version.
project(TriangulatorProj VERSION 0.5)

# Some options for build system generation.
option(DEBUGGING "Toggle debugging mode for symbol generation etc."
           ON)
option(COMPILE_LIBRARY "Compile triangulator core library. If turned off, it is expected to put precompiled triangulator.<ext> library,
                        its headers and dependencies in precompiled/ folder in the uppermost source directory."
           ON)
option(ENABLE_TESTING "Choose whether to compile test files."
           ON)
option(COMPILE_GUI "Compile and use Qt GUI for triangulator."
           ON)

# If we are in debugging mode, toggle debug symbol generation.
if(DEBUGGING)
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

# compiler_flags is used to pass the compiler flags and required CPP
# standard to all targets that include it.
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

# Setting appropriate compiler flags on supported compilers.
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# Set compiler flags for debugging.
if(DEBUGGING)
  target_compile_options(compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wpedantic;-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
  )
  #else() SET UP RELEASE CONFIG
endif()

if(COMPILE_LIBRARY)

  # Generate build system for rapidxml.
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party_software/rapidxml)

  # Generate build system for triangulator library and append it to the list
  # of libraries to be included.
  add_subdirectory(library)
  list(APPEND TRIANG_INCLUDE_LIBS Triangulator)
  
  # Set up project-level installer.
  # For triangulator library:
  #install(TARGETS Triangulator
  #        ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/precompiled/triangulator
  #        )
  #install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/library/include
  #        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/precompiled/triangulator)
  # For rapidxml:
  #install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party_software
  #        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/precompiled
  #        REGEX "CMakeLists" EXCLUDE)

else()

  # If precompiled version of the library and its headers are provided:
  find_library(TRG_PRECOMP NAMES libTriangulator libTriangulator.a libTriangulator.dll
               PATHS ${CMAKE_CURRENT_SOURCE_DIR}/precompiled/triangulator)
  list(APPEND TRIANG_INCLUDE_LIBS ${TRG_PRECOMP})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/precompiled/triangulator/include
                      ${CMAKE_CURRENT_SOURCE_DIR}/precompiled/third_party_software/rapidxml)

endif()

# For now, GUI is not built this way.
if(COMPILE_GUI)
    add_subdirectory(GUI)
endif()

if(ENABLE_TESTING)
  include(CTest)
  add_subdirectory(test)
endif()
