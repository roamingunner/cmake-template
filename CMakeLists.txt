cmake_minimum_required(VERSION 3.10.0)
#详细样例参考 https://github.com/kigster/cmake-project-template  https://github.com/district10/cmake-templates/tree/master/modules 工程
#头文件引用方法参考 https://zhuanlan.zhihu.com/p/82244559

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-module;")

######### import vcpkg 
# Vcpkg helps you manage C and C++ libraries on Windows, Linux and MacOS. This tool and ecosystem are constantly evolving, and we always appreciate contributions!
# website https://github.com/microsoft/vcpkg
# IMPORTANT: change this to where your vcpkg exists.
# IMPORTANT: vcpkg settings must before PROJECT function 

set(VCPKG_HOME "/home/ubuntu/work/vcpkg") 
if(NOT "${VCPKG_HOME}" STREQUAL "")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "arm-linaro")
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${VCPKG_HOME}/scripts/toolchains/arm-linaro-toolchain.cmake")
message(STATUS VCPKG_CHAINLOAD_TOOLCHAIN_FILE=${VCPKG_CHAINLOAD_TOOLCHAIN_FILE})
else()
set(VCPKG_TARGET_TRIPLET "x64-linux")
set(VCPKG_INSTALLED_DIR "vcpkg_install_dir")
include(import_pkg_from_vcpkg)
message(STATUS "finding package from ${CMAKE_PREFIX_PATH}")
endif()

# project start 
project(cmake-template)

# Set the searching location for cmake 'include' locations
include(c++-standards)
include(code-coverage)
include(formatting)
include(sanitizers)
include(dependency-graph)
include(doxygen)
include(Findcppcheck)
include(CppcheckTargets)

######### set cross compile toolchains
# if you import vcpkg ,skip this 
# include(cmake-module/toolchains/toolchain_sample.cmake)



######### set cflags/cxxflags
set(CMAKE_C_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "-O2")

# Require C++11
cxx_11()

# enable build doc
build_docs(PROCESS_DOXYFILE TARGET_NAME doc DOXYFILE_PATH Doxyfile.in)

# generate revision.h and include 
add_custom_target(autorevision 
    COMMAND ${PROJECT_SOURCE_DIR}/utils/script/autorevision.sh -t h > ${PROJECT_SOURCE_DIR}/revision.h
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
include_directories(${PROJECT_SOURCE_DIR})

if(NOT "${VCPKG_HOME}" STREQUAL "")
include_directories("${VCPKG_HOME}/installed/${VCPKG_TARGET_TRIPLET}/include")
else()
include_directories("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
endif()

add_subdirectory(samples/cmake)
add_subdirectory(samples/cppcheck)
add_subdirectory(samples/coverage)
add_subdirectory(samples/sanitizer)
add_subdirectory(samples/check_cpp_version)
add_subdirectory(samples/daemon)
add_subdirectory(samples/client)
add_subdirectory(samples/network)
add_subdirectory(samples/aixlog)
add_subdirectory(samples/libev)
add_subdirectory(samples/nlohmann-json)
add_subdirectory(samples/jsoncpp)
add_subdirectory(samples/catch2)
add_subdirectory(samples/filesystem)
add_subdirectory(samples/boost.asio)
