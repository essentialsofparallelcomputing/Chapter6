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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")

find_package(Vector)

#Pushing vector flags onto compiler flags for try compiles
set(CMAKE_C_FLAGS_SAVE ${CMAKE_C_FLAGS})
set(CMAKE_CXX_FLAGS_SAVE ${CMAKE_CXX_FLAGS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VECTOR_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VECTOR_C_FLAGS}")

try_compile(HAVE_FOG_VECTOR_CLASS "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/fogvectorclasstest.cpp")

if(HAVE_FOG_VECTOR_CLASS)
    message(STATUS "Trying Fog Vector Class -- works")
    add_definitions(-DHAVE_FOG_VECTOR_CLASS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMAX_VECTOR_SIZE=512")
    set (FILE_LIST ${FILE_LIST} 
               kahan_fog_vector.cpp
               kahan_fog_vector8.cpp
               knuth_fog_vector.cpp
               knuth_fog_vector8.cpp)
else()
    message(STATUS "Trying Fog Vector Class -- fails")
endif()


try_compile(HAVE_X86_64_INTRINSICS "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/x86_64_intrinsics_test.c")

if(HAVE_X86_64_INTRINSICS)
    message(STATUS "Trying X86_64 intrinsics -- works")
    add_definitions(-DHAVE_X86_64_INTRINSICS -DMAX_VECTOR_SIZE=512)
    set (FILE_LIST ${FILE_LIST} 
               kahan_intel_vector.c
               kahan_intel_vector8.c
               knuth_intel_vector.c
               knuth_intel_vector8.c)

    try_compile(HAVE_AVX512 "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/AVX512test.c")

    if(HAVE_AVX512)
        message(STATUS "Detecting AVX512 hardware -- found")
        add_definitions(-DHAVE_AVX512)
    else()
        message(STATUS "Detecting AVX512 hardware -- not present")
    endif()

else()
    message(STATUS "Trying X86_64 intrinsics -- fails")
endif()


try_compile(HAVE_GCC_VECTOR_EXTENSIONS "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/gcc_vector_extrinsics_test.c")

if(HAVE_GCC_VECTOR_EXTENSIONS)
    message(STATUS "Trying GCC Vector Extensions -- works")
    add_definitions(-DHAVE_GCC_VECTOR_EXTENSIONS)
    set (FILE_LIST ${FILE_LIST} 
               kahan_gcc_vector.c
               kahan_gcc_vector8.c
               knuth_gcc_vector.c
               knuth_gcc_vector8.c)
else()
    message(STATUS "Trying GCC Vector Extensions -- fails")
endif (HAVE_GCC_VECTOR_EXTENSIONS)


#Popping compiler flags off and back to original state
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_SAVE})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_SAVE})


if(VECTOR_TYPE STREQUAL full_vectorize)
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
