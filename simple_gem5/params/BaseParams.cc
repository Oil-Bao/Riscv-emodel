#include "BaseParams.hh"

// BaseParams 实现
BaseParams::BaseParams(const std::string& componentName) 
    : name(componentName) {}

// ReturnAddrStackParams 实现
ReturnAddrStackParams::ReturnAddrStackParams() 
    : BaseParams("ReturnAddrStack") {
    initialize();
}

void ReturnAddrStackParams::initialize() {
    // 从配置管理器获取参数
    numEntries = getParam<int>("numEntries", 32);
    numThreads = getParam<int>("numThreads", 4);
    traceEnabled = getParam<bool>("traceEnabled", false);
    
    // 打印加载的参数
    std::cout << "=== ReturnAddrStack Parameters ===" << std::endl;
    std::cout << "numEntries: " << numEntries << std::endl;
    std::cout << "numThreads: " << numThreads << std::endl;
    std::cout << "traceEnabled: " << (traceEnabled ? "true" : "false") << std::endl;
    std::cout << "================================" << std::endl;
}