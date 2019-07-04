cmake_minimum_required (VERSION 3.0)
project (globalsums)

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
          -mprefer-vector-width=512 -fopenmp-simd -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info-loop-optimized -fopt-info-loop-missed")
   endif (VECTOR_TYPE STREQUAL "full_vectorize")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel") # using Intel C
   if (VECTOR_TYPE STREQUAL "none")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-vec")
   endif (VECTOR_TYPE STREQUAL "none")
   if (VECTOR_TYPE STREQUAL "full_vectorize")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi-alias -xHost -qopenmp-simd\
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

# Set vectorization flags for a few compilers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") # using Clang
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstrict-aliasing -fvectorize\
       -march=native -mtune=native -Rpass-analysis=loop-vectorize")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") # using GCC
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")
   if (VECTOR_TYPE STREQUAL "none")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-tree-vectorize")
   endif (VECTOR_TYPE STREQUAL "none")
   if (VECTOR_TYPE STREQUAL "full_vectorize")
       set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMAX_VECTOR_SIZE=512 -fstrict-aliasing -ftree-vectorize\
          -mprefer-vector-width=512 -fopenmp-simd -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info-loop-optimized -fopt-info-loop-missed")
   endif (VECTOR_TYPE STREQUAL "full_vectorize")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel") # using Intel C
   if (VECTOR_TYPE STREQUAL "none")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-vec")
   endif (VECTOR_TYPE STREQUAL "none")
   if (VECTOR_TYPE STREQUAL "full_vectorize")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi-alias -xHost -qopenmp-simd\
          -qopt-report=5 -qopt-report-phase=vec,loop")
      if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopt-zmm-usage=high")
      endif ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")
   endif (VECTOR_TYPE STREQUAL "full_vectorize")
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qvec-report:2")
elseif (CMAKE_C_COMPILER_ID MATCHES "XL")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qsimd=auto")
elseif (CMAKE_C_COMPILER_ID MATCHES "Cray")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -h vector3")
endif()

try_compile(HAVE_AVX512 "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/test.c")

if(HAVE_AVX512)
    message("Have AVX512 hardware")
    add_definitions(-DHAVE_AVX512)
else()
    message("Don't Have AVX512 hardware")
endif()

# Adds build target of globalsums with source code files
add_executable(globalsums globalsums.c serial_sum.c kahan_gcc_vector.c kahan_intel_vector.c serial_sum_gcc_vector.c
    serial_sum_intel_vector.c kahan_gcc_vector8.c kahan_intel_vector8.c kahan_sum.c serial_sum_gcc_vector8.c
    serial_sum_intel_vector8.c serial_sum_novec.c kahan_fog_vector8.cpp kahan_fog_vector.cpp serial_sum_fog_vector8.cpp
    serial_sum_fog_vector.cpp)
target_link_libraries(globalsums -lm)
target_include_directories(globalsums PRIVATE vectorclass)

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake ipo_out.optrpt)
