#ifndef RETURN_ADDR_STACK_H
#define RETURN_ADDR_STACK_H

#include "BaseParams.hh"

class ReturnAddrStack {
public:
    // 构造函数使用统一的参数类
    ReturnAddrStack(const ReturnAddrStackParams &p);
    
    // 统计信息
    // struct ReturnAddrStackStats {
    //     ReturnAddrStackStats(int16_t *parent);
    //     int16_t pushes;
    //     int16_t pops;        
    // } stats;
    
    void printConfig() const;
    
private:
    uint numEntries;
    uint numThreads;
    bool traceEnabled;
    
    // 其他成员...
};

#endif // RETURN_ADDR_STACK_H