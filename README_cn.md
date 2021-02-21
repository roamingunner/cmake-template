# c/c++ project template

## 介绍

c/c++ project template是一个c/c++软件开发工程模板,包含如下内容
* cmake构建模板
  * 添加StableCoder module，简化gcc sanitizer，code coverage的使用
  * 添加Doxygen module，支持doxygen开发文档生成
  * 添加autorevision版本戳，支持git svn版本管理
* 包含一些实用的header-only库
  * aixlog
  * CLI11
  * dbg-marco
  * nlohmann-json
* 包含代码开发样例

## cmake构建模板

### 如何使用cmake构建功能
编译二进制、动态库、静态库样例请参考samples/cmake

详细cmake教程参考
* https://github.com/kigster/cmake-project-template 
* https://github.com/district10/cmake-templates/tree/master/modules

### 通过doxygen生成接口文档

* 系统中安装doxygen
* 使能BUILD_DOCUMENTATION。cmake -DBUILD_DOCUMENTATION=ON
* make doc

TIP:
 - 通过调整dockerfile.in的INPUT变量指定需要生成接口文档的文件
 - 修改该文档，README_cn.md 则修改文档首页说明


### 使用autorevision添加版本时间戳

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

### 如何使用gcc sanitizer检查代码问题

These are used by declaring the `USE_SANITIZER` CMake variable as one of:
- Address
- Undefined
- Thread
- Address;Undefined
- Leak

`cmake -D USE_SANITIZER=Address ..`

### 使用Coverage计算代码覆盖率

* 系统中安装ccov命令
* 添加需要计算Coverage的target，参考samples/coverage
* 在cmake配置时使能CODE_COVERAGE `cmake -DCODE_COVERAGE=ON ..`
* make ccov
* 结果生成在ccov目录


### 使用cppcheck静态分析与分析报告

采用并修改CppcheckTargets模块，通过add_cppcheck以及add_cppcheck_sources函数可以自定义需要进行静态分析的代码
```
#  add_cppcheck(<target-name> [UNUSED_FUNCTIONS] [STYLE] [POSSIBLE_ERROR] [FORCE] [FAIL_ON_WARNINGS]) -
#    Create a target to check a target's sources with cppcheck and the indicated options
#  add_cppcheck_sources(<target-name> [UNUSED_FUNCTIONS] [STYLE] [POSSIBLE_ERROR] [FORCE] [FAIL_ON_WARNINGS] <sources list>) -
#    Create a target to check standalone sources with cppcheck and the indicated options
```
add_cppcheck函数用于分析target相关的代码，add_cppcheck_sources函数可以创建一个target来分析自定义的代码，具体请参考samples的cppcheck样例。

* make all_cppcheck #分析所有add_cppcheck以及add_cppcheck_sources指定的所有代码
* make all_cppcheck_report #生成cppcheck分析报告，报告存放在cppcheck_report目录

### 如何交叉编译

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

参考samples目录

