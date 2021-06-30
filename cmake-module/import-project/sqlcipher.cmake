message("External project: sqlcipher")
ExternalProject_Add(sqlcipher
    URL https://github.com/sqlcipher/sqlcipher/archive/refs/tags/v4.4.3.tar.gz
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND <SOURCE_DIR>/configure 
        --prefix=<INSTALL_DIR>
        --host=${CMAKE_COMPILER_PREFIX}
        --enable-tempstore=yes
        --disable-tcl
        CFLAGS=-DSQLITE_HAS_CODEC
        LDFLAGS=-lcrypto
        CC=${CMAKE_C_COMPILER}
        CXX=${CMAKE_CXX_COMPILER}
)
