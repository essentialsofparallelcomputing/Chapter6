cmake_minimum_required (VERSION 3.0)
project (stream_triad)

# Set vectorization flags for a few compilers
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") # using Clang
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -fvectorize\
       -march=native -Rpass-analysis=loop-vectorize")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") # using GCC
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -ftree-vectorize\
       -march=native -fopt-info-vec-optimized -fopt-info-loop-optimized")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel") # using Intel C
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi-alias -vec -xHost\
       -qopt-report-phase=vec,loop")
endif()

# Adds build target of stream_triad with source code files
add_executable(stream_triad stream_triad.c timer.c timer.h)

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake stream_triad.dSYM)

