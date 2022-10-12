message("Import libarchive")
ExternalProject_Add(libarchive
    URL xxxxx
    BUILD_IN_SOURCE 1
    PREFIX ${PROJECT_SOURCE_DIR}/depends
    CONFIGURE_COMMAND <SOURCE_DIR>/configure 
        --host=${CMAKE_COMPILER_PREFIX}
        --prefix=<INSTALL_DIR>
        --without-iconv
        --without-bz2lib
        --without-lz4
        --without-lzma
        --without-openssl
        --without-xml2
        --without-expat
        PKG_CONFIG_PATH=${PROJECT_SOURCE_DIR}/depends/lib/pkgconfig CFLAGS=${CMAKE_C_FLAGS} CXXFLAGS=${CMAKE_CXX_FLAGS} CPPFLAGS=-I${PROJECT_SOURCE_DIR}/depends/include LDFLAGS=-L${PROJECT_SOURCE_DIR}/depends/lib
    INSTALL_COMMAND make install
)
