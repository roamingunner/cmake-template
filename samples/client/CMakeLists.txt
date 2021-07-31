add_executable(argp argp.c)
add_executable(getopt getopt.c)

find_package(CLI11 CONFIG REQUIRED)
add_executable(geet-cli11 geet-cli11.cpp)
target_link_libraries(geet-cli11 PRIVATE CLI11::CLI11)
