cmake_minimum_required(VERSION 3.10.0)
#详细样例参考 https://github.com/kigster/cmake-project-template  https://github.com/district10/cmake-templates/tree/master/modules 工程
project(exmample)

#cross compile example. for detail https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html
# set(CMAKE_SYSTEM_NAME Linux)
# set(CMAKE_SYSTEM_PROCESSOR arm)

#set(CMAKE_SYSROOT /home/devel/rasp-pi-rootfs)
#set(CMAKE_STAGING_PREFIX /home/devel/stage)

# set(tools /home/leo/.local/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf)
# set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
# set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set the searching location for cmake 'include' locations
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-module;")

set(CMAKE_C_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "-O2")

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

add_custom_target(autorevision 
    COMMAND ${PROJECT_SOURCE_DIR}/external/autorevision/autorevision.sh -t h > ${PROJECT_SOURCE_DIR}/revision.h
)

add_subdirectory(samples/cmake)
add_subdirectory(samples/aixlog)
add_subdirectory(samples/sanitizer)
add_subdirectory(samples/client)
add_subdirectory(samples/daemon)
add_subdirectory(samples/network)
add_subdirectory(samples/libev)
add_subdirectory(samples/nlohmann-json)
add_subdirectory(samples/boost.asio)
add_subdirectory(samples/boost.thread)