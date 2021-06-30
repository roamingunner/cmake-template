message("External project: openssl")

ExternalProject_Add(openssl
    URL https://www.openssl.org/source/openssl-1.1.1i.tar.gz
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND 
    "AR=${CMAKE_COMPILER_PREFIX}-ar"
    "AS=${CMAKE_COMPILER_PREFIX}-as"
    "LD=${CMAKE_COMPILER_PREFIX}-ld"
    "NM=${CMAKE_COMPILER_PREFIX}-nm"
    "CC=${CMAKE_COMPILER_PREFIX}-gcc"
    "GCC=${CMAKE_COMPILER_PREFIX}-gcc"
    "CPP=${CMAKE_COMPILER_PREFIX}-cpp"
    "CXX=${CMAKE_COMPILER_PREFIX}-g++"
    "FC=${CMAKE_COMPILER_PREFIX}-gfortran"
    "F77=${CMAKE_COMPILER_PREFIX}-gfortran"
    "RANLIB=${CMAKE_COMPILER_PREFIX}-ranlib"
    "READELF=${CMAKE_COMPILER_PREFIX}-readelf"
    "STRIP=${CMAKE_COMPILER_PREFIX}-strip"
    "OBJCOPY=${CMAKE_COMPILER_PREFIX}-objcopy"
    "OBJDUMP=${CMAKE_COMPILER_PREFIX}-objdump"
    "DEFAULT_ASSEMBLER=${CMAKE_COMPILER_PREFIX}-as"
    "DEFAULT_LINKER=${CMAKE_COMPILER_PREFIX}-ld"
    "CFLAGS=-I${PROJECT_SOURCE_DIR}/depends/include"
    "CXXFLAGS=-I${PROJECT_SOURCE_DIR}/depends/include"
    "LDFLAGS=-L${PROJECT_SOURCE_DIR}/depends/lib"
    "FCFLAGS=-Os"
    "FFLAGS=-Os"
    ./Configure linux-armv4 --prefix=<INSTALL_DIR> -latomic -lpthread threads shared  no-rc5 enable-camellia enable-mdc2 no-tests no-fuzz-libfuzzer no-fuzz-afl zlib-dynamic
    BUILD_COMMAND make
    INSTALL_COMMAND make install_sw
)
