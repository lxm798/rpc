file(GLOB SRCS "*.cpp")
#set(SRCS SRC)

add_library(lockfree11 ${SRCS})
target_link_libraries(lockfree11 pthread)
set_target_properties(lockfree11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS lockfree11 DESTINATION lib)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#  add_subdirectory(tests)
#endif()
