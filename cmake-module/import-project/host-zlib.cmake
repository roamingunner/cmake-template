message("External project: host-zlib")

ExternalProject_Add(host-zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG "v1.2.11"
    GIT_SHALLOW 1
    BUILD_IN_SOURCE 1
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/host
        -DBUILD_SHARED_LIBS=ON 
)