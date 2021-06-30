message("External project: host-abseil")

ExternalProject_Add(host-abseil
    BUILD_IN_SOURCE 1
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG "20210324.2"
    GIT_SHALLOW 1
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/host
    -DCMAKE_CXX_COMPILER_ID=GNU
    -DCMAKE_CXX_STANDARD=11
    -DABSL_ENABLE_INSTALL=ON
    -DABSL_USE_GOOGLETEST_HEAD=OFF
    -DABSL_RUN_TESTS=OFF
    -DBUILD_SHARED_LIBS=ON 
)

# maybe cmake has a bug. project() will motify value of CMAKE_SYSTEM_PROCESSOR
#