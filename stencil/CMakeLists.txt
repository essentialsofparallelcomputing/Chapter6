cmake_minimum_required (VERSION 3.0)
project (stencil)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (CMAKE_C_STANDARD 99)

if (NOT VECTOR_TYPE)
   set (VECTOR_TYPE full_vectorize)
endif (NOT VECTOR_TYPE)
if (VECTOR_TYPE STREQUAL "none")
   set (VECTOR_TYPE none)
endif (VECTOR_TYPE STREQUAL "none")
if (VECTOR_TYPE STREQUAL "default")
   set (VECTOR_TYPE default)
endif (VECTOR_TYPE STREQUAL "default")

# Set vectorization flags for a few compilers
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") # using Clang
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -fvectorize\
       -march=native -mtune=native -Rpass-analysis=loop-vectorize")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") # using GCC
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native")
   if (VECTOR_TYPE STREQUAL "none")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-tree-vectorize")
   endif (VECTOR_TYPE STREQUAL "none")
   if (VECTOR_TYPE STREQUAL "full_vectorize")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -ftree-vectorize\
          -mprefer-vector-width=512 -fopt-info-vec-optimized")
          # needed for some versions of the GCC compiler to generate AVX512 instructions
          #-mprefer-vector-width=512
   endif (VECTOR_TYPE STREQUAL "full_vectorize")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel") # using Intel C
   if (VECTOR_TYPE STREQUAL "none")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-vec")
   endif (VECTOR_TYPE STREQUAL "none")
   if (VECTOR_TYPE STREQUAL "full_vectorize")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi-alias -xHost\
          -qopt-report=5 -qopt-report-phase=vec,loop")
      if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qopt-zmm-usage=high")
      endif ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")
   endif (VECTOR_TYPE STREQUAL "full_vectorize")
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Qvec-report:2")
elseif (CMAKE_C_COMPILER_ID MATCHES "XL")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsimd=auto")
elseif (CMAKE_C_COMPILER_ID MATCHES "Cray")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -h vector3")
endif()

find_package(likwid)

# Adds build target of stencil with source code files
add_executable(stencil stencil.c timer.c timer.h malloc2d.c malloc2d.h)

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake stencil.dSYM ipo_out.optrpt)
