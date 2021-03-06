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


include_directories(${PROJECT_BINARY_DIR})
set(SRCS
    main.cpp echo.pb.cc)
add_executable(exe ${SRCS})
target_link_libraries(exe utils protobuf protocol gflags reactor11)
set_target_properties(exe PROPERTIES COMPILE_FLAGS "-std=c++11")

set(queue
    lock_free_queue_test.cpp)
add_executable(lock_free_queue ${queue})
target_link_libraries(lock_free_queue lockfree11)
set_target_properties(lock_free_queue PROPERTIES COMPILE_FLAGS "-std=c++11")

set(glog_src glog.cpp)
add_executable(glog_test ${glog_src})
target_link_libraries(glog_test glog::glog)

set(gflags_src gflags.cpp gflags_declare_flags.cc)
add_executable(gflags_test ${gflags_src})
target_link_libraries(gflags_test gflags)

set(client_src
    rpcclient.cpp echo.pb.cc)
add_executable(client ${client_src})
target_link_libraries(client utils protobuf protocol gflags rpcclient11)
set_target_properties(client PROPERTIES COMPILE_FLAGS "-std=c++11")

set(stress_src
    stressclient.cpp echo.pb.cc)
add_executable(stress ${stress_src})
target_link_libraries(stress utils protobuf protocol gflags rpcclient11)
set_target_properties(stress PROPERTIES COMPILE_FLAGS "-std=c++11")
configure_file(conf  ./)


