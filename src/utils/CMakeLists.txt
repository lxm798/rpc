file(GLOB SRCS "*.cpp")
#set(SRCS SRC)

add_library(utils ${SRCS})
target_link_libraries(utils pthread)
set_target_properties(utils PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS utils DESTINATION lib)
