cmake_minimum_required(VERSION 2.6)
set(C_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 # -DCHECK_PTHREAD_RETURN_VALUE
 # -D_FILE_OFFSET_BITS=64
 #-Wall
 # -Wextra
 #-Werror
 # -Wconversion
 #-Wno-unused-parameter
 #-Wold-style-cast
 # -Woverloaded-virtual
 -Wpointer-arith
 #-Wshadow
 #-Wwrite-strings
 #-march=native
 # -MMD
 # -std=c++0x
 #-rdynamic
 )

set(CMAKE_C_COMPILER "g++")
set(SRCS
    coroutine.c)
add_library(coroutine  ${SRCS})
set(HEADERS
    coroutine.h)
INSTALL(FILES ${HEADERS} DESTINATION include/coroutine)
