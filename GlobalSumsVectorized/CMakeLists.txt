cmake_minimum_required (VERSION 3.0)
project (globalsums)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set (CMAKE_C_STANDARD 99)

find(Vector)

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

if(VECTOR_TYPE full_vectorize)
    if (CMAKE_VECTOR_VERBOSE)
        set(VECTOR_C_FLAGS "${VECTOR_C_FLAGS} ${VECTOR_C_VERBOSE}")
        set(VECTOR_CXX_FLAGS "${VECTOR_CXX_FLAGS} ${VECTOR_CXX_VERBOSE}")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VECTOR_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VECTOR_CXX_FLAGS}")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VECTOR_NOVEC_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VECTOR_NOVEC_CXX_FLAGS}")
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
                  Makefile cmake_install.cmake globalsums.dSYM ipo_out.optrpt)
