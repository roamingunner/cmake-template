cmake_minimum_required(VERSION 3.10.0)
project(NANOPB_CMAKE_SIMPLE C)

include(FetchContent)
FetchContent_Declare(
  nanopb
  GIT_REPOSITORY https://github.com/nanopb/nanopb.git
  GIT_TAG        nanopb-0.4.7 # release-1.11.0
)

FetchContent_MakeAvailable(nanopb)
set(CMAKE_MODULE_PATH ${nanopb_SOURCE_DIR}/extra)
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})

nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS cmd.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")

add_executable(static_pb_test static_pb_test.c ${PROTO_SRCS} ${PROTO_HDRS})

