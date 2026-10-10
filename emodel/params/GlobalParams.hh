
#ifndef __GLOBAL_PARAMS_HH__
#define __GLOBAL_PARAMS_HH__

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "CacheParams.hh"
#include "CoreParams.hh"

namespace py = pybind11;

void bind_common_types(py::module_ &m);
void config_module_init(py::module_ &m_internal);

class BaseCPUParams {
public:
    CacheConfig L1I;
    CacheConfig L1D;
    IFUParams IFU;
    void print_config() const;
};

namespace Global{
    
    extern BaseCPUParams GlobalParams;
    struct WorkloadInfo {
        std::string elfPath;
        std::map<uint64_t, uint32_t> memoryData;
        Addr entry_point;
        WorkloadInfo() : elfPath("default.elf") {}

        // 获取数据
        uint32_t getData(uint64_t addr) const {
            auto it = memoryData.find(addr);
            return (it != memoryData.end()) ? it->second : 0;
        }
    };
    extern WorkloadInfo workload;
}

#endif