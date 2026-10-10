
# 最小 CMake 版本要求
cmake_minimum_required(VERSION 3.14)

# 定义项目名称为 'add'，并指定项目使用 C++ 语言。
project(add LANGUAGES CXX)

# 查找 Python 3 环境。
# COMPONENTS Interpreter Development: 确保找到 Python 解释器和开发头文件/库。
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# 查找 pybind11 库。
# REQUIRED: 如果找不到 pybind11，则停止配置并报错。
add_subdirectory("/home/model/emodel/simple_gem5/pybind11" pybind11)
#find_package(pybind11 REQUIRED)

# 设置 C++ 语言标准为 C++11。pybind11 通常至少需要 C++11。
set(CMAKE_CXX_STANDARD 11)
# 强制要求编译器必须支持设置的 C++ 标准
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 设置默认构建类型为 Release。这会启用优化选项。
set(CMAKE_BUILD_TYPE Release)
# 为 Release 构建类型设置高级优化标志 (-O3)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# 使用 pybind11 提供的便捷函数来创建 Python 扩展模块。
# 目标名: add (对应于 Python 中 import add)
# 源文件: add.cpp
# pybind11 会自动处理链接 Python 库和添加必要的包含目录。
pybind11_add_module(add add.cpp init.cc test_config.cc)