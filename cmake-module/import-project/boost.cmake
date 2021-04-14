message( "External project: boost" )
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE TOOLCHAIN_VERSION)
set(BOOST_WITH_LIBS "libraries = --with-chrono --with-headers --with-system --with-test --with-timer ;")
string(STRIP ${TOOLCHAIN_VERSION} TOOLCHAIN_VERSION_STRIP)
file(WRITE ${PROJECT_SOURCE_DIR}/cmake-module/import-project/gen_user_config.sh "echo \"using gcc : ${TOOLCHAIN_VERSION_STRIP} : ${CMAKE_CXX_COMPILER}  : \<cxxflags\>\"-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os \";\n${BOOST_WITH_LIBS}\" > $1")

ExternalProject_Add(boost
  URL "xxxxxx"
  BUILD_IN_SOURCE 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ./bootstrap.sh
  COMMAND /bin/sh ${PROJECT_SOURCE_DIR}/cmake-module/import-project/gen_user_config.sh <SOURCE_DIR>/user-config.jam
  BUILD_COMMAND  ./b2
    --user-config=./user-config.jam
    --prefix=<INSTALL_DIR>
    threading=multi
    link=shared
    runtime-link=shared
    variant=release
    toolset=gcc
    abi=aapcs
    -d+1
    install
  INSTALL_COMMAND ""
