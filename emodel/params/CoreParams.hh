#ifndef CORE_PARAMS_HH
#define CORE_PARAMS_HH

#include <iostream>
#include <cstdint>
#include "BaseParams.hh"
#include "src/base/types.hh"

//ReturnAddrStack
struct RASParams {
    RASParams() : numEntries(0), numThreads(0), traceEnabled(false) {}
    
    // 使用模板方法动态获取参数
    int numEntries;
    int numThreads;
    bool traceEnabled;
};

class IFUParams  : public BaseParams {
public:
    RASParams RAS;
    unsigned FetchWidth;
    unsigned DecodeWidth;
    Addr BootPC;
    
    IFUParams() : FetchWidth(0), DecodeWidth(0), BootPC(0) {}
};
#endif // CORE_PARAMS_HH