include_directories(${PROJECT_BINARY_DIR})
set(SRCS
    main.cpp)
add_executable(exe ${SRCS})
target_link_libraries(exe reactor11)
set_target_properties(exe PROPERTIES COMPILE_FLAGS "-std=c++0x")

set(queue
    lock_free_queue_test.cpp)
add_executable(lock_free_queue ${queue})
target_link_libraries(lock_free_queue lockfree11)
set_target_properties(lock_free_queue PROPERTIES COMPILE_FLAGS "-std=c++0x")
