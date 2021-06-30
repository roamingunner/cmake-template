message("External project: host-grpc")
# reference https://stackoverflow.com/questions/52202453/cross-compiling-grpc-using-cmake

ExternalProject_Add(host-grpc
    URL https://github.com/grpc/grpc/archive/refs/tags/v1.38.0.tar.gz
    CMAKE_ARGS +=
            -DgRPC_INSTALL:BOOL=ON
            -DgRPC_BUILD_TESTS:BOOL=OFF
            -DCMAKE_BUILD_TYPE=Release
            -DgRPC_ABSL_PROVIDER=package
            -DgRPC_CARES_PROVIDER=package
            -DgRPC_PROTOBUF_PROVIDER=package
            -DgRPC_PROTOBUF_PACKAGE_TYPE=MODULE
            -DProtobuf_PROTOC_EXECUTABLE=<INSTALL_DIR>/host/bin/protoc
            -DgRPC_RE2_PROVIDER=package
            -DgRPC_SSL_PROVIDER=package
            -DgRPC_ZLIB_PROVIDER=package
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/host
            -DCMAKE_PREFIX_PATH=<INSTALL_DIR>/host/lib/cmake
            -DCMAKE_C_FLAGS=-I<INSTALL_DIR>/host/include
            -DCMAKE_CXX_FLAGS=-I<INSTALL_DIR>/host/include
            -DCMAKE_CXX_IMPLICIT_LINK_DIRECTORIES=-I<INSTALL_DIR>/host/lib
            -DCMAKE_C_IMPLICIT_LINK_DIRECTORIES=-I<INSTALL_DIR>/host/lib
    DEPENDS host-cares host-protobuf host-re2 host-zlib host-abseil
)