#ifndef BASEPARAMS_H
#define BASEPARAMS_H

#include <string>
#include <memory>
#include "ConfigManager.hh"

// 参数基类
class BaseParams {
public:
    BaseParams(const std::string& componentName);
    virtual ~BaseParams() = default;
    
    std::string name;
    
    // 获取参数的通用方法
    template<typename T>
    T getParam(const std::string& paramName, T defaultValue = T()) {
        return ConfigManager::getInstance().getParameter<T>(name, paramName, defaultValue);
    }
};

// ReturnAddrStack 特定参数
struct ReturnAddrStackParams : public BaseParams {
    ReturnAddrStackParams();
    
    // 使用模板方法动态获取参数
    int numEntries;
    int numThreads;
    bool traceEnabled;
    
    void initialize();
};

#endif // BASEPARAMS_H