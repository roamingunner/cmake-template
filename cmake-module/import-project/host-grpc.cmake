message("External project: grpc")
# reference https://stackoverflow.com/questions/52202453/cross-compiling-grpc-using-cmake

ExternalProject_Add(grpc
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
            -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_C_FLAGS=-I<INSTALL_DIR>/include
            -DCMAKE_CXX_FLAGS=-I<INSTALL_DIR>/include
            -DCMAKE_CXX_IMPLICIT_LINK_DIRECTORIES=-I<INSTALL_DIR>/lib
            -DCMAKE_C_IMPLICIT_LINK_DIRECTORIES=-I<INSTALL_DIR>/lib
    DEPENDS cares host-protobuf protobuf re2 zlib abseil
)