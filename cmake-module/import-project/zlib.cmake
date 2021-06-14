message("External project: zlib")

ExternalProject_Add(zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG "v1.2.11"
    GIT_SHALLOW 1
    BUILD_IN_SOURCE 1
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
)