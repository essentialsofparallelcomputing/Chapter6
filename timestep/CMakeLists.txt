cmake_minimum_required (VERSION 3.0)
project (timestep)

set (CMAKE_C_STANDARD 99)

# Set vectorization flags for a few compilers
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") # using Clang
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstrict-aliasing -fvectorize\
       -march=native -mtune=native -Rpass-analysis=loop-vectorize")

elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") # using GCC
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native\
       -fstrict-aliasing -ftree-vectorize -fopenmp-simd\
       -fopt-info-vec-optimized -fopt-info-vec-missed")
   if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "7.9.0")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mprefer-vector-width=512")
   endif ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER "7.9.0")

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

# Adds build target of timestep with source code files
add_executable(timestep main.c timestep.c)
target_link_libraries(timestep m)

add_executable(timestep_opt main.c timestep_opt.c)
target_link_libraries(timestep_opt m)

add_executable(timestep_opt1 main.c timestep_opt1.c)
target_link_libraries(timestep_opt1 m)

add_executable(timestep_opt2 main.c timestep_opt2.c)
target_link_libraries(timestep_opt2 m)

add_executable(timestep_opt3 main.c timestep_opt3.c)
target_link_libraries(timestep_opt3 m)


# Cleanup
add_custom_target(distclean COMMAND rm -rf CMakeCache.txt CMakeFiles
                  Makefile cmake_install.cmake timestep.dSYM ipo_out.optrpt)
