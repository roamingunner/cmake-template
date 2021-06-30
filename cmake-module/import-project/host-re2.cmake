message("External project: host-re2")

ExternalProject_Add(host-re2
    GIT_REPOSITORY https://github.com/google/re2.git
    GIT_TAG "2020-06-01"
    GIT_SHALLOW 1
    BUILD_IN_SOURCE 1
    CMAKE_ARGS += -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/host
)