#include "ras.hh"
#include <iostream>

ReturnAddrStack::ReturnAddrStack(const ReturnAddrStackParams &p)
    : numEntries(p.numEntries),
      numThreads(p.numThreads),
      traceEnabled(p.traceEnabled)
      //stats(nullptr)  // 根据实际需要初始化
{
    // 打印配置信息
    std::cout << "\n[ReturnAddrStack Constructor]" << std::endl;
    std::cout << "numEntries = " << numEntries << std::endl;
    std::cout << "numThreads = " << numThreads << std::endl;
    std::cout << "traceEnabled = " << traceEnabled << std::endl;
    
    // DPRINTF(RAS, "Create RAS stacks with %d threads, %d entries each.\n", 
    //         numThreads, numEntries);
    
    // 初始化代码
    for (unsigned i = 0; i < numThreads; ++i) {
        // addrStacks.emplace_back(*this);
        // addrStacks[i].init(numEntries);
        std::cout << "dddddd" << std::endl;
    }
}

void ReturnAddrStack::printConfig() const {
    std::cout << "\n=== ReturnAddrStack Current Config ===" << std::endl;
    std::cout << "Entries per thread: " << numEntries << std::endl;
    std::cout << "Number of threads: " << numThreads << std::endl;
    std::cout << "Tracing enabled: " << (traceEnabled ? "Yes" : "No") << std::endl;
}