include_directories(${PROJECT_SOURCE_DIR}/samples/cmake/include ${PROJECT_SOURCE_DIR})

add_library(foo STATIC foo/foo.c)
add_library(bar SHARED bar/bar.c)
add_executable(cmake-sample main.c)
target_link_libraries(cmake-sample foo bar)
ADD_DEPENDENCIES(cmake-sample autorevision)

