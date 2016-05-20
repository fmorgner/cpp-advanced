find_package(Threads)
cute_test(BoundedQueue)
target_link_libraries(BoundedQueue_test ${CMAKE_THREAD_LIBS_INIT})
