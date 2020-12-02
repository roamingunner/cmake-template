# cmake template <!-- omit in toc -->

this is a c/c++ project template , it include some useful feature to make coding  more stable and efficient

 * include [cmake-scripts](https://github.com/StableCoder/cmake-scripts) helping you sanitize(thread/address/memory/...) project
 * add Doxygen file. helo you build source docs
 * add autorevision project to make version stamp
 * add dbg-macro

## instruction

### basic 

- git clone --recurse-submodules https://github.com/roamingunner/cmake-template.git
- mkdir build && cd build
- cmake ..
- make # build example

### build with sanitizer

These are used by declaring the `USE_SANITIZER` CMake variable as one of:
- Address
- Memory
- MemoryWithOrigins
- Undefined
- Thread
- Address;Undefined
- Undefined;Address
- Leak

`cmake -D USE_SANITIZER=Address ..`

### build doc

doxygen will scan file in src directory defaultly. you can modify this by motify INPUT variant in Doxyfile.in 

`make doc`

