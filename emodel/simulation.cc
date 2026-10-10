#include <pybind11/pybind11.h>
#include "sim/readelf.hh"
#include "src/core/ifu.hh"
#include "src/core/decode.hh"
#include "params/GlobalParams.hh"
#include "base/init.hh"
#include <iostream>
#include <cstdint>

int simulation() {
    extract_sections(Global::workload.elfPath);

    const auto& GlobalParams = Global::GlobalParams;

    // 全局控制信号缓冲区：足够保存过去 3 周期 + 未来 0 周期的数据
    TimeBuffer<ControlSignals> ctrlBuf(3, 0);

    // 数据流缓冲区
    TimeBuffer<FetchStruct> fetchToDecode(0, 1);
    TimeBuffer<FetchStruct> decodeToExecute(0, 1);
    TimeBuffer<FetchStruct> executeToCommit(0, 1);

    // 创建阶段
    ///Fetch Fetch(GlobalParams);
    Fetch Fetch(GlobalParams,ctrlBuf);
    Decode Decode(fetchToDecode);
    Execute Execute(decodeToExecute, ctrlBuf);
    Commit Commit(executeToCommit, ctrlBuf);

    const int CYCLES = 8;

    std::cout << "=== Pipeline with Feedback (Flush Example) ===\n";
    for (int cyc = 0; cyc < CYCLES; ++cyc) {
        std::cout << "\n--- Cycle " << cyc << " ---\n";

        // Tick 各阶段（顺序重要：先上游，再下游）
        Fetch.tick();
        Decode.tick();
        Execute.tick();
        Commit.tick();

        // 推进所有缓冲区
        Fetch.output.advance();        // Fetch → Decode
        fetchToDecode.advance();       // 实际应连接，这里简化
        Decode.output.advance();       // Decode → Execute
        decodeToExecute.advance();
        executeToCommit.advance();

        ctrlBuf.advance();             // 控制信号时间推进
    }

    

    std::cout << "ELF 文件: " << Global::workload.elfPath << std::endl;

    std::cout << "\n=== Starting Simulation ===" << std::endl;
    std::cout << "l1i cache way's  " << GlobalParams.L1I.assoc << std::endl;
    
    // ... 仿真代码 ...
    

    return 0;
}


// 在 simulation.cc 的 PYBIND11_MODULE 块内，或 init.hh 的 initAll 函数中
PYBIND11_MODULE(emodel, module) {
    module.doc() = "CPU Simulation Module";
    
    // 1. 先绑定所有公共类型到主模块
    bind_common_types(module);
    
    // 2. 初始化子模块（现在子模块可能更简单了）
    EmbeddedPyBindSimple::initAll(module); // 这个函数内部会调用 config_module_init
    
    // 3. 暴露全局实例。注意：现在 IFU 成员返回的类型在主模块已注册
    module.attr("Params") = &Global::GlobalParams;
    module.attr("workload") = &Global::workload;
    // 4. 暴露仿真函数
    module.def("simulation", &simulation, "start simulation");
}

