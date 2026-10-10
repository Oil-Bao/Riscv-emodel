# 最小 CMake 版本要求
cmake_minimum_required(VERSION 3.14)

# 定义项目，同时支持 C 和 C++ 语言
project(emodel LANGUAGES C CXX)

# 查找 Python 3 环境
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# 添加 pybind11
add_subdirectory("/home/model/emodel/emodel/pybind11" pybind11)

# 设置语言标准
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 查找所有源文件
file(GLOB_RECURSE ALL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

# 过滤掉不需要的文件（可选）
# 或者直接使用明确的文件列表
set(SOURCE_FILES
    simulation.cc
    src/core/ifu.cc
    params/AllParams.cc
    src/base/init.cc
    src/sim/readelf.cc
)

# 添加 libelf C 文件
file(GLOB LIBELF_C_FILES "src/sim/libelf/*.c")
list(APPEND SOURCE_FILES ${LIBELF_C_FILES})

# 创建模块
pybind11_add_module(emodel ${SOURCE_FILES})

# 设置包含目录
target_include_directories(emodel PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/params
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sim/libelf
    ${CMAKE_CURRENT_SOURCE_DIR}/pybind11/include
    ${Python3_INCLUDE_DIRS}
)

# 打印信息
message(STATUS "Total source files: ${SOURCE_FILES}")