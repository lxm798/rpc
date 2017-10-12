cmake_minimum_required(VERSION 2.6)
project(rpc C CXX)
enable_testing()
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 # -Wconversion
 -Wno-unused-parameter
 #-Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 #-Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 # -std=c++0x
 -rdynamic
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost REQUIRED)
find_package(glog 0.3.5 REQUIRED)
find_package(gflags REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
set(cur_dir "./include")
include_directories(${cur_dir})
set(third_dir "./third-lib")
include_directories(${third-lib})
set(glog_dir "./third-lib/glog")
include_directories(${glog_dir})
set(gflags "./third-lib/gflags/build/include")
include_directories(${gflags})
set(coroutine "./third-lib/coroutine")
include_directories(${coroutine})
set(proto "./proto")
include_directories(${proto})
add_subdirectory(third-lib/coroutine)
#add_subdirectory(third-lib/glog)
#add_subdirectory(include)
add_subdirectory(proto)
add_subdirectory(src/utils)
add_subdirectory(src/lock_free)
add_subdirectory(src/net)
add_subdirectory(src/client)
add_subdirectory(test)

