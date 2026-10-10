#ifndef __CONFIG_HH__
#define __CONFIG_HH__

#include <string>

// 替换策略枚举
enum class ReplacePolicy {
    RR,
    PLRU, 
    LRU,
    Num_ReplacePolicy
};

// Cache配置
struct CacheConfig {
    int size;                // 字节
    int associativity;       // 路数
    ReplacePolicy policy;    // 替换策略
    
    CacheConfig(int size = 65536, 
                int assoc = 8, 
                ReplacePolicy policy = ReplacePolicy::LRU)
        : size(size), associativity(assoc), policy(policy) {}
};

// 仿真器配置
struct SimConfig {
    std::string name;        // 仿真名称
    int cycles;              // 周期数
    CacheConfig cache;       // Cache配置
    int verbosity;           // 详细级别
    bool trace_enabled;      // 是否启用追踪
    
    SimConfig(const std::string& name = "default_sim",
              int cycles = 1000000,
              const CacheConfig& cache = CacheConfig(),
              int verbosity = 1,
              bool trace_enabled = false)
        : name(name), cycles(cycles), cache(cache),
          verbosity(verbosity), trace_enabled(trace_enabled) {}
};

#endif // __CONFIG_HH__