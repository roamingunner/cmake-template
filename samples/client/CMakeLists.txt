add_executable(argp argp.c)
add_executable(getopt getopt.c)
add_executable(geet-cli11 geet-cli11.cpp)
target_include_directories(geet-cli11 PRIVATE ${PROJECT_SOURCE_DIR}/external/CLI11)
