#include "config.hh"
#include "base/init.hh"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// 绑定函数：配置相关
static void config_module_init(py::module_ &m_internal) {
    // 创建config子模块
    py::module_ m = m_internal.def_submodule("config");
    
    // 绑定ReplacePolicy枚举
    py::enum_<ReplacePolicy>(m, "ReplacePolicy")
        .value("RR", ReplacePolicy::RR)
        .value("PLRU", ReplacePolicy::PLRU)
        .value("LRU", ReplacePolicy::LRU)
        .value("Num_ReplacePolicy", ReplacePolicy::Num_ReplacePolicy)
        .export_values();
    
    // 绑定CacheConfig结构体
    py::class_<CacheConfig>(m, "CacheConfig")
        .def(py::init<>())
        .def(py::init<int, int, ReplacePolicy>(),
             py::arg("size") = 65536,
             py::arg("associativity") = 8,
             py::arg("policy") = ReplacePolicy::LRU)
        .def_readwrite("size", &CacheConfig::size)
        .def_readwrite("associativity", &CacheConfig::associativity)
        .def_readwrite("policy", &CacheConfig::policy)
        .def("__repr__", [](const CacheConfig &c) {
            return "CacheConfig(size=" + std::to_string(c.size) + 
                   ", assoc=" + std::to_string(c.associativity) + ")";
        });
    
    // 绑定SimConfig结构体
    py::class_<SimConfig>(m, "SimConfig")
        .def(py::init<>())
        .def(py::init<const std::string&, int, const CacheConfig&, int, bool>(),
             py::arg("name") = "default_sim",
             py::arg("cycles") = 1000000,
             py::arg("cache") = CacheConfig(),
             py::arg("verbosity") = 1,
             py::arg("trace_enabled") = false)
        .def_readwrite("name", &SimConfig::name)
        .def_readwrite("cycles", &SimConfig::cycles)
        .def_readwrite("cache", &SimConfig::cache)
        .def_readwrite("verbosity", &SimConfig::verbosity)
        .def_readwrite("trace_enabled", &SimConfig::trace_enabled)
        .def("__repr__", [](const SimConfig &s) {
            return "SimConfig(name='" + s.name + 
                   "', cycles=" + std::to_string(s.cycles) + ")";
        });
}

// 自动注册
SIMPLE_PYBIND(config, config_module_init);
