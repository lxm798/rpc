file(GLOB SRCS "*.cpp")
#set(SRCS SRC)

find_package(glog 0.3.5 REQUIRED)

add_library(rpcclient11 ${SRCS})
target_link_libraries(rpcclient11 reactor11)
set_target_properties(rpcclient11 PROPERTIES COMPILE_FLAGS "-std=c++11")

install(TARGETS rpcclient11 DESTINATION lib)
