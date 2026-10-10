#include "test_config.hh"
#include "init.hh"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// 定义全局配置
SimConfig GlobalConfig::global_config;

// 实现GlobalConfig函数
SimConfig& GlobalConfig::get_config() {
    return global_config;
}

void GlobalConfig::set_config(const SimConfig& config) {
    global_config = config;
}

// 实现SimConfig的成员函数
void SimConfig::print_config() const {
    std::cout << "Cache Config:" << std::endl;
    std::cout << "  Size: " << cache_config.size << " bytes" << std::endl;
    std::cout << "  Associativity: " << cache_config.assoc << std::endl;
    std::cout << "  Block Size: " << cache_config.block_size << " bytes" << std::endl;
    std::cout << "  Replace Policy: ";
    switch(cache_config.replace_policy) {
        case ReplacePolicy::RR: std::cout << "RR"; break;
        case ReplacePolicy::PLRU: std::cout << "PLRU"; break;
        case ReplacePolicy::LRU: std::cout << "LRU"; break;
        default: std::cout << "Unknown";
    }
    std::cout << std::endl;
}

// 绑定函数：配置相关
void config_module_init(py::module_ &m_internal) {
    py::module_ module = m_internal.def_submodule("config");
    
    // 暴露枚举类型
    py::enum_<ReplacePolicy>(module, "ReplacePolicy")
        .value("RR", ReplacePolicy::RR)
        .value("PLRU", ReplacePolicy::PLRU)
        .value("LRU", ReplacePolicy::LRU)
        .value("Num_ReplacePolicy", ReplacePolicy::Num_ReplacePolicy)
        .export_values();
    
    // 暴露CacheConfig结构体
    py::class_<CacheConfig>(module, "CacheConfig")
        .def(py::init<>())
        .def_readwrite("size", &CacheConfig::size)
        .def_readwrite("assoc", &CacheConfig::assoc)
        .def_readwrite("block_size", &CacheConfig::block_size)
        .def_readwrite("replace_policy", &CacheConfig::replace_policy);
    
    // 暴露SimConfig类
    py::class_<SimConfig>(module, "SimConfig")
        .def(py::init<>())
        .def_readwrite("cache_config", &SimConfig::cache_config)
        .def("print_config", &SimConfig::print_config);
}

// 使用宏注册绑定
SIMPLE_PYBIND(config, config_module_init);