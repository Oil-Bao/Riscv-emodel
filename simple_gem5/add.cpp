#include <pybind11/pybind11.h>
#include "test_config.hh"
#include "init.hh"
#include <iostream>
#include <cstdint>

// 使用全局配置的add函数
int add(int a, int b) {
    const auto& config = GlobalConfig::get_config();
    std::cout << "Add: Using cache size = " << config.cache_config.size << std::endl;
    std::cout << "Replace policy = ";
    switch(config.cache_config.replace_policy) {
        case ReplacePolicy::RR: std::cout << "RR"; break;
        case ReplacePolicy::PLRU: std::cout << "PLRU"; break;
        case ReplacePolicy::LRU: std::cout << "LRU"; break;
        default: std::cout << "Unknown";
    }
    std::cout << std::endl;
    
    return a + b + config.cache_config.size;
}

// 使用全局配置的sub函数
int sub(int a, int b) {
    const auto& config = GlobalConfig::get_config();
    std::cout << "Sub: Using cache size = " << config.cache_config.size << std::endl;
    std::cout << "Block size======= Sub******* = " << config.cache_config.block_size << std::endl;
    
    return a - b - config.cache_config.size;
}

// 重置为默认配置
void reset_config() {
    GlobalConfig::set_config(SimConfig());
}

PYBIND11_MODULE(add, module) {
    module.doc() = "pybind11 example plugin";
    
    EmbeddedPyBindSimple::initAll(module);
    
    // 暴露操作全局配置的函数
    module.def("set_global_config", [](const SimConfig& config) {
        GlobalConfig::set_config(config);
    }, "Set global configuration");
    
    module.def("get_global_config", []() -> SimConfig& {
        return GlobalConfig::get_config();
    }, pybind11::return_value_policy::reference_internal,
       "Get global configuration");
    
    module.def("reset_config", &reset_config, "Reset to default configuration");
    
    // 暴露计算函数
    module.def("add", &add, "Add two numbers using global config");
    module.def("sub", &sub, "Subtract two numbers using global config");
}