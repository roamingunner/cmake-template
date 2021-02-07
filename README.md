# c/c++ project template <!-- omit in toc -->
## instruction
this is a c/c++ project template , it include some useful feature to make coding more stable and efficient.
* include cmake template
  * include [cmake-scripts](https://github.com/StableCoder/cmake-scripts) helping you sanitize(thread/address/memory/...) project
  * add Doxygen file. helo you build source docs
  * add autorevision project to make version stamp
* collect some useful header-only libraries
  * aixlog
  * CLI11
  * dbg-marco
  * nlohmann-json
* sample code

## cmake template

### cmake basic sample

please reference samples/cmake, more detail 
* https://github.com/kigster/cmake-project-template 
* https://github.com/district10/cmake-templates/tree/master/modules
### build with sanitizer

These are used by declaring the `USE_SANITIZER` CMake variable as one of:
- Address
- Undefined
- Thread
- Address;Undefined
- Undefined;Address
- Leak

`cmake -D USE_SANITIZER=Address ..`

### build doc

doxygen will scan file in src directory defaultly. you can modify this by motify INPUT variant in Doxyfile.in 

`make doc`


### cross compile

cross compile example. for detail https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html

```
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT /home/devel/rasp-pi-rootfs)
set(CMAKE_STAGING_PREFIX /home/devel/stage)

set(tools /home/devel/gcc-4.7-linaro-rpi-gnueabihf)
set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
```
