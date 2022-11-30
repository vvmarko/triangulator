# Setting minimum required version for CMake.
cmake_minimum_required(VERSION 3.15)

# Project name and version.
project(Triangulator VERSION 0.5)

# Some options for build system generation.
option(DEBUGGING "Toggle debugging mode for symbol generation etc." ON)
option(COMPILE_LIBRARY "Compile triangulator core library. If turned off, it is expected to put precompiled triangulator.<ext> library
                        and its headers in precompiled_libs/triangulator folder." OFF)
option(ENABLE_TESTING "Choose whether to compile test files." ON)
option(ENABLE_GUI "Compile and use Qt GUI for triangulator." OFF)

# If we are in debugging mode, toggle debug symbol generation.
if(DEBUGGING)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# compiler_flags is used to pass the compiler flags and required CPP
# standard to all targets that include it.
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

# Setting appropriate compiler flags on supported compilers.
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

if(COMPILE_LIBRARY)
  # Generate build system for rapidxml.
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party_software/rapidxml)

  # Generate build system for triangulator library and append it to the list
  # of libraries to be included.
  add_subdirectory(library)
  list(APPEND TRIANG_INCLUDE_LIBS TriangulatorLib)
else()
  # If precompiled version of the library and its headers are provided.
  find_library(TRG_PRECOMP NAMES Triangulato Triangulator.a Triangulator.dll
               PATHS ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_libs/triangulator NO_DEFAULT_PATH)
  list(APPEND TRIANG_INCLUDE_LIBS ${TRG_PRECOMP})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/precompiled_libs/triangulator ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_libs/rapidxml)
endif()

# For now, GUI is not built this way.
if(ENABLE_GUI)
    add_subdirectory(GUI)
endif()

if(ENABLE_TESTING)
  add_subdirectory(test)
endif()