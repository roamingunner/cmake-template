message("External project: libev")
ExternalProject_Add(libev
    URL http://dist.schmorp.de/libev/Attic/libev-4.33.tar.gz
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)