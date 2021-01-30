find_package(Boost REQUIRED COMPONENTS thread)
add_executable(boost_thread thread.cpp)
target_link_libraries(boost_thread Boost::thread)
add_executable(boost_thread_pool thread_pool.cpp)
target_link_libraries(boost_thread_pool Boost::thread)

