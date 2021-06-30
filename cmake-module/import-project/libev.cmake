message("External project: libev")

ExternalProject_Add(libev
    URL http://dist.schmorp.de/libev/Attic/libev-4.33.tar.gz
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND <SOURCE_DIR>/configure 
        --prefix=<INSTALL_DIR> 
        --host=${CMAKE_COMPILER_PREFIX}
        --enable-shared
        --disable-static
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
)