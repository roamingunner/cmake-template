cmake_minimum_required(VERSION 3.10.0)
#详细样例参考 https://github.com/kigster/cmake-project-template  https://github.com/district10/cmake-templates/tree/master/modules 工程
project(exmample)

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