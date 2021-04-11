# set cross compile toolchains
#cross compile example. for detail https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(toolchains_path /home/leo/.local/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf)
set(CMAKE_C_COMPILER ${toolchains_path}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${toolchains_path}/bin/arm-linux-gnueabihf-g++)
string(REPLACE "-gcc" "" CMAKE_COMPILER_PREFIX "${CMAKE_C_COMPILER}")
