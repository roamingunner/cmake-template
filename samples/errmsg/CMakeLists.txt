add_library(xxapi STATIC xxapi.c errmsg.c)
add_executable(test-errmsg cli.c)
target_link_libraries(test-errmsg xxapi)