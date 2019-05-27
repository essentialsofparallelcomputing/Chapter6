cmake_minimum_required (VERSION 3.0)
project (stream_triad)

set (CMAKE_C_STANDARD 99)

# Set vectorization flags for a few compilers
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") # using Clang
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -fvectorize\
       -march=native -Rpass-analysis=loop-vectorize")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") # using GCC
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -ftree-vectorize\
       -march=native -mprefer-vector-width=512 -fopt-info-vec-optimized")
       # needed for some versions of the GCC compiler to generate AVX512 instructions
       #-mprefer-vector-width=512
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel") # using Intel C
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi-alias -xHost\
       -qopt-report=5 -qopt-report-phase=vec,loop")
   if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qopt-zmm-usage=high")
   endif ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Qvec-report:2")
elseif (CMAKE_C_COMPILER_ID MATCHES "XL")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsimd=auto")
elseif (CMAKE_C_COMPILER_ID MATCHES "Cray")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -h vector3")
endif()

# Adds build target of stream_triad with source code files
add_executable(stream_triad stream_triad.c timer.c timer.h)

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake stream_triad.dSYM ipo_out.optrpt)
