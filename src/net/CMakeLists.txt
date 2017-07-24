file(GLOB SRCS "*.cpp")
#set(SRCS SRC)

add_library(reactor ${SRCS})
target_link_libraries(reactor pthread coroutine)

add_library(reactor11 ${SRCS})
target_link_libraries(reactor11 pthread coroutine)
set_target_properties(reactor11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS reactor DESTINATION lib)
install(TARGETS reactor11 DESTINATION lib)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#  add_subdirectory(tests)
#endif()
