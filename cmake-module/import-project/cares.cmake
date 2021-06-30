message("External project: cares")

ExternalProject_Add(cares
    GIT_REPOSITORY https://github.com/c-ares/c-ares.git
    GIT_TAG "cares-1_15_0"
    GIT_SHALLOW 1
    BUILD_IN_SOURCE 1
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
    -DBUILD_SHARED_LIBS=ON 
)