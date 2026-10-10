#ifndef TEST_CONFIG_HH
#define TEST_CONFIG_HH

#include <iostream>
#include <cstdint>

enum class ReplacePolicy {
    RR,
    PLRU, 
    LRU,
    Num_ReplacePolicy
};

struct CacheConfig {
    uint32_t size = 32768;  // 32KB
    uint32_t assoc = 8;
    uint32_t block_size = 64;
    ReplacePolicy replace_policy = ReplacePolicy::LRU;
};

class SimConfig {
public:
    CacheConfig cache_config;
    
    void print_config() const;
};

// 声明全局配置对象
namespace GlobalConfig {
    // 声明全局可访问的配置
    extern SimConfig global_config;
    
    // 声明函数
    SimConfig& get_config();
    void set_config(const SimConfig& config);
}

#endif // TEST_CONFIG_HH