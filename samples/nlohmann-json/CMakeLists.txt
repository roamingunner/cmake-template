
find_package(nlohmann_json CONFIG REQUIRED)
add_executable(nlohmann_json_sample nlohmann_json_sample.cpp)
target_link_libraries(nlohmann_json_sample PRIVATE nlohmann_json nlohmann_json::nlohmann_json)



