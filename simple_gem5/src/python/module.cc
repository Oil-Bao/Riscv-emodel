#include "base/init.hh"
#include <pybind11/pybind11.h>

#include <pybind11/pybind11.h>

int add(int i, int j) {
	return i + j;
}

PYBIND11_MODULE(add, module) {
	module.doc() = "pybind11 example plugin"; //optional module docstring
	module.def("add", &add, "A function which adds two numbers");
}

// namespace py = pybind11;

// PYBIND11_MODULE(gem5_sim, m) {
//     m.doc() = "Gem5-like simulator with simplified PyBind11";
    
//     // 初始化所有自动注册的绑定
//     EmbeddedPyBindSimple::initAll(m);
    
//     // 添加一些全局函数
//     m.def("create_default_simulator", []() {
//         return py::module_::import("gem5_sim")
//                      .attr("SimConfig")();
//     });
    
//     m.def("print_info", []() {
//         py::print("=== Gem5 Simulator ===");
//         py::print("Version: 1.0.0");
//         py::print("Config auto-binding enabled");
//     });
// }
