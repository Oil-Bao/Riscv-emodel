#include "ifu.hh"
#include "src/base/static_inst_fwd.hh"
#include "src/base/dyn_inst.hh"
#include "params/GlobalParams.hh"


static InstSeqNum globalSeqNum = 0;

Fetch::Fetch(const BaseCPUParams &params,TimeBuffer<ControlSignals>& ctrl)
    :FetchWidth(params.IFU.FetchWidth),
     DecodeWidth(params.IFU.DecodeWidth),
     BootPC(params.IFU.BootPC),
     output(0, 1), 
     ctrlBuf(ctrl)
{

    fromCommit = ctrlBuf.getWire(-commitToFetchDelay);

    std::cout << "fetch width is " << FetchWidth << std::endl;
    std::cout << "boot pc is " << BootPC << std::endl;
    //std::cout << "boot pc is " << Global::workload.getData(0x10178) << std::endl;
    std::cout << "boot pc is 0x" << std::hex << std::setw(8) << std::setfill('0') 
          << Global::workload.getData(0x10390) << std::dec << std::endl;
}

void Fetch::tick() {
    if (stalled || cycle >= (int)program.size()) {
        *output.getWire(1) = FetchStruct{};
        return;
    }

    // microOps = new StaticInstPtr[numMicroops];

    // StaticInstPtr *uop = microOps;

    // 1. 获取静态指令（模拟 decoder）

    OpClass opc;
    opc = IntAlu;
    StaticInstPtr static_inst = new StaticInst(program[cycle].c_str(),opc); // RefCountingPtr 自动管理

    // 2. 分配全局序列号
    InstSeqNum seq_num = globalSeqNum++;

    // 3. 创建动态指令（通过工厂）
    DynInstPtr dyn_inst = DynInst::create(static_inst, seq_num);

    // 4. 填充 FetchStruct
    FetchStruct fs;
    fs.inst = dyn_inst;   // RefCountingPtr 赋值，自动 incref/decref
    fs.pc = pc;
    fs.valid = true;

    pc += 4;
    std::cout << "[Cycle] Fetch: " << fs.inst->getMnemonic()
              << " (seq=" << fs.inst->getSeqNum() << ") @ PC=" << fs.pc << "\n";

    *output.getWire(1) = fs;
    cycle++;
}