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

file(GLOB SRCS "*.cpp")
#set(SRCS SRC)

find_package(glog 0.3.5 REQUIRED)
#add_library(reactor ${SRCS})
#target_link_libraries(reactor pthread coroutine glog::glog)

add_library(reactor11 ${SRCS})
target_link_libraries(reactor11 pthread coroutine protocol glog::glog protocol)
set_target_properties(reactor11 PROPERTIES COMPILE_FLAGS "-std=c++11")

#install(TARGETS reactor DESTINATION lib)
install(TARGETS reactor11 DESTINATION lib)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#  add_subdirectory(tests)
#endif()
