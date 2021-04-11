message("External project: googletest")

ExternalProject_Add(googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.10.0.tar.gz
  CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_BUILD_TYPE=Release
    -Dgtest_force_shared_crt=OFF
)