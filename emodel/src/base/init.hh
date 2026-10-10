
#ifndef __INIT_HH__
#define __INIT_HH__

#include <pybind11/pybind11.h>
#include <functional>
#include <string>
#include <vector>

namespace py = pybind11;

/**
 * 简化版EmbeddedPyBind
 * 功能：自动收集所有绑定函数，在initAll时一次性执行
 */
class EmbeddedPyBindSimple {
private:
    // 存储所有绑定函数
    static std::vector<std::pair<std::string, 
                                 std::function<void(py::module_&)>>>& getBindings();
    
public:
    /**
     * 构造函数，注册绑定函数
     * @param name 模块名称（仅用于调试）
     * @param func 绑定函数
     */
    EmbeddedPyBindSimple(const std::string& name, 
                         std::function<void(py::module_&)> func);
    
    /**
     * 执行所有注册的绑定
     * @param m Python模块
     */
    static void initAll(py::module_& m);
};

// 简化注册宏
#define SIMPLE_PYBIND(name, func) \
        static EmbeddedPyBindSimple pybind_##name(#name, func); \


#endif // __INIT_HH__