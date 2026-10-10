#include "init.hh"



// 获取全局绑定列表
std::vector<std::pair<std::string, 
                      std::function<void(py::module_&)>>>& 
EmbeddedPyBindSimple::getBindings() {
    static std::vector<std::pair<std::string, 
                                 std::function<void(py::module_&)>>> bindings;
    return bindings;
}



// 构造函数：注册绑定函数
EmbeddedPyBindSimple::EmbeddedPyBindSimple(
    const std::string& name, 
    std::function<void(py::module_&)> func) {
    
    // 简单地将绑定函数添加到列表中
    getBindings().emplace_back(name, func);
}

// 执行所有注册的绑定
void EmbeddedPyBindSimple::initAll(py::module_& m) {
    auto& bindings = getBindings();
    
    // 遍历执行所有绑定函数
    for (auto& [name, func] : bindings) {
        func(m);  // 执行绑定函数
    }
}