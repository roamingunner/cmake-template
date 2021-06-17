message("External project: host-protobuf")

ExternalProject_Add(host-protobuf
    URL https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.15.8.tar.gz
    BUILD_IN_SOURCE 1
    SOURCE_SUBDIR "cmake"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/host
    -DCMAKE_BUILD_TYPE:STRING=release
    -Dprotobuf_BUILD_TESTS=OFF
    -Dprotobuf_WITH_ZLIB:BOOL=OFF
)