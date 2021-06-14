message("External project: re2")

ExternalProject_Add(re2
    GIT_REPOSITORY https://github.com/google/re2.git
    GIT_TAG "2020-06-01"
    GIT_SHALLOW 1
    BUILD_IN_SOURCE 1
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
)