message("Import libfuse")
ExternalProject_Add(libfuse
    URL xxxx
    BUILD_IN_SOURCE 1
    PREFIX ${PROJECT_SOURCE_DIR}/depends
    CONFIGURE_COMMAND <SOURCE_DIR>/configure 
        --host=${CMAKE_COMPILER_PREFIX}
        --enable-lib
        --enable-util
        --disable-example
        --disable-debug
        --prefix=<INSTALL_DIR>
        PKG_CONFIG_PATH=${PROJECT_SOURCE_DIR}/depends/lib/pkgconfig CFLAGS=${CMAKE_C_FLAGS} CXXFLAGS=${CMAKE_CXX_FLAGS} CPPFLAGS=-I${PROJECT_SOURCE_DIR}/depends/include LDFLAGS=-L${PROJECT_SOURCE_DIR}/depends/lib
    INSTALL_COMMAND make -C <SOURCE_DIR>/lib install
    COMMAND make -C <SOURCE_DIR>/include install
    COMMAND make install-pkgconfigDATA
)
