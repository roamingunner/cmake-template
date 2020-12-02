add_custom_target(autorevision 
    COMMAND ${PROJECT_SOURCE_DIR}/external/autorevision/autorevision.sh -t h > ${PROJECT_SOURCE_DIR}/samples/cmake-sample/include/revision.h
)
include_directories(${PROJECT_SOURCE_DIR}/samples/cmake-sample/include)

add_library(foo STATIC foo/foo.c)
add_library(bar SHARED bar/bar.c)
add_executable(example main.c)
target_link_libraries(example foo bar)
ADD_DEPENDENCIES(example autorevision)

