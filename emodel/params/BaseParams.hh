#ifndef BASE_PARAMS_HH
#define BASE_PARAMS_HH

#include <string>

class BaseParams {
public:
    // 添加默认构造函数
    BaseParams() = default;
    
    // 虚析构函数
    virtual ~BaseParams() = default;
    
    // 可以添加其他构造函数
    BaseParams(const BaseParams&) = default;
    BaseParams& operator=(const BaseParams&) = default;
};


enum class ReplacePolicy {
    RR,
    PLRU, 
    LRU
};

#endif