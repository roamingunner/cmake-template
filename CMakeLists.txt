cmake_minimum_required(VERSION 3.10.0)
#详细样例参考 https://github.com/kigster/cmake-project-template  https://github.com/district10/cmake-templates/tree/master/modules 工程
project(exmample)

#cross compile example. for detail https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR arm)

#set(CMAKE_SYSROOT /home/devel/rasp-pi-rootfs)
#set(CMAKE_STAGING_PREFIX /home/devel/stage)

#set(tools /home/devel/gcc-4.7-linaro-rpi-gnueabihf)
#set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
#set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set the searching location for cmake 'include' locations
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-module;")

include(c++-standards)
include(code-coverage)
include(formatting)
include(sanitizers)
include(tools)
include(dependency-graph)
include(doxygen)

# Require C++11
cxx_11()

build_docs(PROCESS_DOXYFILE TARGET_NAME doc DOXYFILE_PATH Doxyfile.in)

add_subdirectory(samples/cmake-sample)
add_subdirectory(samples/aixlog-sample)
