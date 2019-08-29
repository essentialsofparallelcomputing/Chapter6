cmake_minimum_required (VERSION 3.0)
project (mass_sum)

set (CMAKE_C_STANDARD 99)

# Set vectorization flags for a few compilers
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") # using Clang
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -fvectorize\
       -march=native -mtune=native -ffast-math -Rpass-analysis=loop-vectorize")

elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") # using GCC
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -ftree-vectorize -fopenmp-simd\
       -march=native -mtune=native -ffast-math -fopt-info-vec-all -fopt-info-loop-all")
   if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "7.4.0")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mprefer-vector-width=512")
   endif ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "7.4.0")

elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel") # using Intel C
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi-alias -xHost\
       -qopt-report=5 -qopt-report-phase=vec,loop")
   if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qopt-zmm-usage=high")
   endif ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "17.0.4")

elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Qvec-report:2")

elseif (CMAKE_C_COMPILER_ID MATCHES "XL")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsimd=auto")

elseif (CMAKE_C_COMPILER_ID MATCHES "Cray")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -h vector3")

endif()

# Adds build target of mass_sum with source code files
add_executable(mass_sum main.c mass_sum.c)

target_link_libraries(mass_sum m)

# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake mass_sum.dSYM ipo_out.optrpt)
