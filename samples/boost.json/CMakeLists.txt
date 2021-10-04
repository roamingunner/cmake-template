
find_package(Boost REQUIRED json)
add_executable(pretty_print pretty.cpp)
target_link_libraries(pretty_print PRIVATE Boost::boost Boost::json)
add_executable(json json.cpp)
target_link_libraries(json PRIVATE Boost::boost Boost::json)
add_executable(json2 json2.cpp)
target_link_libraries(json2 PRIVATE Boost::boost Boost::json)


