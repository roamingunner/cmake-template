add_executable(aixlog_example aixlog_example.cpp)

find_path(AIXLOG_INCLUDE_DIRS "aixlog.hpp")
target_include_directories(aixlog_example PRIVATE ${AIXLOG_INCLUDE_DIRS})

