message("External project: protobuf")

ExternalProject_Add(protobuf
    URL https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.15.8.tar.gz
    BUILD_IN_SOURCE 1
    SOURCE_SUBDIR "cmake"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_BUILD_TYPE:STRING=release
    -Dprotobuf_BUILD_TESTS=OFF
    -Dprotobuf_WITH_ZLIB:BOOL=OFF
)