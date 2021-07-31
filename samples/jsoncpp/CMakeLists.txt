add_executable(jsoncpp_sample jsoncpp_sample.cpp)
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(jsoncpp_sample PRIVATE jsoncpp_lib)
