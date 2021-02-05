# project template

## 说明

project template是一个c/c++软件开发工程模板，为c/c++软件包开发提供了如下方便的特性来

* 提供CMakeList.txt撰写模板
* 添加StableCoder module，简化gcc sanitizer，code coverage的使用
* 添加Doxygen module，支持doxygen开发文档生成
* 添加autorevision版本戳，支持git svn版本管理
* 包含一些实用的header-only库
  * aixlog
  * CLI11
  * dbg-marco
  * nlohmann-json
* 包含代码开发样例

## 使用cmake管理工程

编译二进制、动态库、静态库样例请参考samples/cmake

详细cmake教程参考
* https://github.com/kigster/cmake-project-template 
* https://github.com/district10/cmake-templates/tree/master/modules

## 通过doxygen生成接口文档

* 系统中安装doxygen
* 使能BUILD_DOCUMENTATION。cmake -DBUILD_DOCUMENTATION=ON
* make doc

TIP:
 - 通过调整dockerfile.in的INPUT变量指定需要生成接口文档的文件
 - 修改该文档，README_cn.md 则修改文档首页说明


## 使用autorevision添加版本时间戳

顶层CMakefile.txt已经定了autorevision target，工程会自动申城revision.h版本头文件
```
add_custom_target(autorevision 
    COMMAND ${PROJECT_SOURCE_DIR}/external/autorevision/autorevision.sh -t h > ${PROJECT_SOURCE_DIR}/revision.h
)
```

如果一个目标需要引用autorevision 生成的revision.h头文件，请参考samples/cmake样例，为目标添加DEPENDS。以确保revision.h会在目标之前生成
```
ADD_DEPENDENCIES(cmake-sample autorevision)
```

## 如何使用gcc sanitizer检查代码问题

These are used by declaring the `USE_SANITIZER` CMake variable as one of:
- Address
- Undefined
- Thread
- Address;Undefined
- Undefined;Address
- Leak

`cmake -D USE_SANITIZER=Address ..`

## 使用Coverage计算代码覆盖率

* 系统中安装ccov命令
* 添加需要计算Coverage的target，参考samples/coverage
* 在cmake配置时使能CODE_COVERAGE `cmake -DCODE_COVERAGE=ON ..`
* make ccov
* 结果生成在ccov目录

## 如何交叉编译

cmake交叉编译的详细设定参考https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html

针对本工程模板，需要根据目标情况修改顶层CMakeList.txt ,设置工具链参数，样例如下
```
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT /home/devel/rasp-pi-rootfs)
set(CMAKE_STAGING_PREFIX /home/devel/stage)

set(tools /home/leo/.local/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf)
set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)
```

## 代码开发样例

