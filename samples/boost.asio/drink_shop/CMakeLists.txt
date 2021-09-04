find_package(jsoncpp CONFIG REQUIRED)
add_executable(drink_shop drink_shop_main.cpp drink_shop_receptionist.cpp drink_shop_config.cpp drink_shop_transaction.cpp)
target_link_libraries(drink_shop PRIVATE pthread jsoncpp)
add_executable(test_protocol test_protocol.cpp)
