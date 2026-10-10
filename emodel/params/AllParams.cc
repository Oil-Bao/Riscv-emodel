// AllParams.cc
#include "GlobalParams.hh"
#include "base/init.hh"

// 定义全局配置
BaseCPUParams Global::GlobalParams;
Global::WorkloadInfo Global::workload; 

// === 核心：将类型的公共绑定抽离到一个函数中，供主模块和子模块调用 ===
void bind_common_types(py::module_ &m) {
    // 在主模块 m 中绑定 IFUParams
    py::class_<IFUParams>(m, "IFUParams")
        .def(py::init<>())
        .def_readwrite("FetchWidth", &IFUParams::FetchWidth)
        .def_readwrite("DecodeWidth", &IFUParams::DecodeWidth)
        .def_readwrite("BootPC", &IFUParams::BootPC);

    // 在主模块 m 中绑定 CacheConfig
    py::class_<CacheConfig>(m, "CacheConfig")
        .def(py::init<>())
        .def_readwrite("size", &CacheConfig::size)
        .def_readwrite("assoc", &CacheConfig::assoc)
        .def_readwrite("block_size", &CacheConfig::block_size)
        .def_readwrite("replace_policy", &CacheConfig::replace_policy);

    // 在主模块 m 中绑定 BaseCPUParams
    py::class_<BaseCPUParams>(m, "BaseCPUParams")
        .def(py::init<>())
        .def_readwrite("IFU", &BaseCPUParams::IFU)
        .def_readwrite("L1I", &BaseCPUParams::L1I)
        .def_readwrite("L1D", &BaseCPUParams::L1D);

    // 在主模块 m 中绑定 WorkloadInfo
    py::class_<Global::WorkloadInfo>(m, "WorkloadInfo")
        .def(py::init<>())
        .def_readwrite("elfPath", &Global::WorkloadInfo::elfPath);
        //.def_readwrite("memoryData", &Global::WorkloadInfo::memoryData)
        //.def("getData", &Global::WorkloadInfo::getData, py::arg("addr"),
        //     "Get data from memory at specified address");
}

// 绑定函数：config子模块相关 (现在只绑定枚举或子模块特有的东西)
void config_module_init(py::module_ &m_internal) {
    py::module_ module = m_internal.def_submodule("config");
    
    // 暴露枚举类型 (可以留在子模块)
    py::enum_<ReplacePolicy>(module, "ReplacePolicy")
        .value("RR", ReplacePolicy::RR)
        .value("PLRU", ReplacePolicy::PLRU)
        .value("LRU", ReplacePolicy::LRU)
        .export_values();
    
    // 注意：我们不再在子模块中重复定义 IFUParams 等类
    // 而是可以通过 module.attr("IFUParams") = m_internal.attr("IFUParams"); 来复用
    // 但更简单的方法是让用户直接从主模块导入：simulation.IFUParams
}