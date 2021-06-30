message("External project: host-cares")

ExternalProject_Add(host-cares
    GIT_REPOSITORY https://github.com/c-ares/c-ares.git
    GIT_TAG "cares-1_15_0"
    GIT_SHALLOW 1
    BUILD_IN_SOURCE 1
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/host -DBUILD_SHARED_LIBS=ON 
    
)