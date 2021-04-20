include_directories(${PROJECT_SOURCE_DIR}/samples/cmake/include ${PROJECT_SOURCE_DIR})

add_library(foo STATIC foo/foo.c)
add_library(bar SHARED bar/bar.c)
add_executable(cmake-sample main.c)
target_link_libraries(cmake-sample foo bar)
ADD_DEPENDENCIES(cmake-sample autorevision)

# install DESTINATION must be rpath
# reference https://zhuanlan.zhihu.com/p/141956373
# using COMPONENT can package compoment in different package. like "runtime" and "developmet"
install(TARGETS cmake-sample DESTINATION bin COMPONENT run)
install(TARGETS bar DESTINATION lib COMPONENT dev)
install(TARGETS foo DESTINATION lib COMPONENT dev)
install(FILES include/bar.h DESTINATION include COMPONENT dev)
