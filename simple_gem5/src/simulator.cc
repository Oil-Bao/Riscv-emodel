#include "config.hh"
#include "base/init.hh"
#include <pybind11/pybind11.h>
#include <iostream>

namespace py = pybind11;

// 仿真器类
class Simulator {
private:
    SimConfig config;
    int current_cycle;
    
public:
    Simulator(const SimConfig& config) 
        : config(config), current_cycle(0) {}
    
    void run() {
        std::cout << "Starting simulation: " << config.name << std::endl;
        std::cout << "Cycles: " << config.cycles << std::endl;
        std::cout << "Cache size: " << config.cache.size << " bytes" << std::endl;
        
        // 模拟运行
        for (int i = 0; i < config.cycles; i++) {
            if (i % 100000 == 0 && config.verbosity > 0) {
                std::cout << "Cycle: " << i << std::endl;
            }
        }
        
        std::cout << "Simulation finished!" << std::endl;
    }
    
    void update_config(const SimConfig& new_config) {
        config = new_config;
        std::cout << "Config updated" << std::endl;
    }
    
    SimConfig get_config() const {
        return config;
    }
};

namespace {

// 绑定函数：仿真器相关
static void simulator_module_init(py::module_ &m_internal) {
    py::module_ m = m_internal.def_submodule("simulator");
    
    // 绑定Simulator类
    py::class_<Simulator>(m, "Simulator")
        .def(py::init<const SimConfig&>())
        .def("run", &Simulator::run)
        .def("update_config", &Simulator::update_config)
        .def("get_config", &Simulator::get_config)
        .def("__repr__", [](const Simulator &s) {
            auto config = s.get_config();
            return "Simulator(name='" + config.name + "')";
        });
}

// 自动注册
SIMPLE_PYBIND(simulator, simulator_module_init);

} // 匿名命名空间