cmake_minimum_required(VERSION 3.10.0)
#详细样例参考 https://github.com/kigster/cmake-project-template  https://github.com/district10/cmake-templates/tree/master/modules 工程
#头文件引用方法参考 https://zhuanlan.zhihu.com/p/82244559

project(c-cpp-project-template)

# Set the searching location for cmake 'include' locations
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-module;")
include(c++-standards)
include(code-coverage)
include(formatting)
include(sanitizers)
include(dependency-graph)
include(doxygen)
include(Findcppcheck)
include(CppcheckTargets)

######### set cross compile toolchains
include(cmake-module/toolchains/toolchain_sample.cmake)

######### set cflags/cxxflags
set(CMAKE_C_FLAGS "-O2")
set(CMAKE_CXX_FLAGS "-O2")

######### import external projects
include(ExternalProject)
set_property(DIRECTORY PROPERTY EP_PREFIX ${PROJECT_SOURCE_DIR}/depends)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/depends/lib/cmake;${CMAKE_MODULE_PATH}")
link_directories(${PROJECT_SOURCE_DIR}/depends/lib)
include_directories(${PROJECT_SOURCE_DIR}/depends/include)
include(cmake-module/import-project/abseil.cmake)
include(cmake-module/import-project/cares.cmake)
include(cmake-module/import-project/re2.cmake)
include(cmake-module/import-project/zlib.cmake)
include(cmake-module/import-project/host-protobuf.cmake)
include(cmake-module/import-project/protobuf.cmake)
include(cmake-module/import-project/openssl.cmake)
include(cmake-module/import-project/grpc.cmake)

###############################################

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

# add_subdirectory(samples/cmake)
# add_subdirectory(samples/aixlog)
# add_subdirectory(samples/sanitizer)
# add_subdirectory(samples/client)
# add_subdirectory(samples/daemon)
# add_subdirectory(samples/network)
# add_subdirectory(samples/libev)
# add_subdirectory(samples/nlohmann-json)
# add_subdirectory(samples/jsoncpp)
# add_subdirectory(samples/check_cpp_version)
# add_subdirectory(samples/coverage)
# add_subdirectory(samples/cppcheck)
# add_subdirectory(samples/filesystem)
# find_package(Boost)
# if(Boost_FOUND)
# add_subdirectory(samples/boost.asio)
# add_subdirectory(samples/boost.thread)
# endif(Boost_FOUND)
# add_subdirectory(samples/catch2)
