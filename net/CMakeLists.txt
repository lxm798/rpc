file(GLOB SRC "src/*.cpp")
set(SRCS SRC)

add_library(reactor ${SRCS})
target_link_libraries(reactor pthread rt)

add_library(reactor11 ${SRCS})
target_link_libraries(reactor11 pthread rt)
set_target_properties(reactor11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS muduo_base DESTINATION lib)
install(TARGETS muduo_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "include/*.h")
install(FILES ${HEADERS} DESTINATION include/net/)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
  add_subdirectory(tests)
endif()
