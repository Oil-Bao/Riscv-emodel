#ifndef CACHE_CONFIG_HH
#define CACHE_CONFIG_HH

#include <iostream>
#include <cstdint>
#include "BaseParams.hh"


struct CacheConfig {
public:
    uint32_t size       = 32768;  // 32KB
    uint32_t assoc      = 8;
    uint32_t block_size = 64;
    ReplacePolicy replace_policy = ReplacePolicy::LRU;
};

#endif // CACHE_CONFIG_HH