// simulator_pybind.cpp
#include "pybind11.h"
#include "stl.h"

namespace py = pybind11;

class Cache {
public:
    int size;
    int associativity;
    ReplacePolicy policy;
    
    Cache(int size, int assoc, ReplacePolicy policy) 
        : size(size), associativity(assoc), policy(policy) {}
};

class Simulator {
public:
    std::string name;
    int cycles;
    Cache cache;
    
    Simulator(std::string name, int cycles, Cache cache)
        : name(name), cycles(cycles), cache(cache) {}
    
    void run() {
        // 运行模拟
        py::print("Running simulation:", name);
        py::print("Cache policy:", cache.policy);
    }
};

PYBIND11_MODULE(simulator, m) {
    py::enum_<ReplacePolicy>(m, "ReplacePolicy")
        .value("RR", ReplacePolicy::RR)
        .value("PLRU", ReplacePolicy::PLRU)
        .value("LRU", ReplacePolicy::LRU)
        .export_values();
    
    py::class_<Cache>(m, "Cache")
        .def(py::init<int, int, ReplacePolicy>())
        .def_readwrite("size", &Cache::size)
        .def_readwrite("associativity", &Cache::associativity)
        .def_readwrite("policy", &Cache::policy);
    
    py::class_<Simulator>(m, "Simulator")
        .def(py::init<std::string, int, Cache>())
        .def("run", &Simulator::run)
        .def_readwrite("name", &Simulator::name)
        .def_readwrite("cycles", &Simulator::cycles)
        .def_readwrite("cache", &Simulator::cache);
}