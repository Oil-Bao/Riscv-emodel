#include "src/base/types.hh"
#include "src/base/common.hh"
#include "src/base/timebuf.hh"
#include "src/base/dyn_inst.hh"

// struct InstructionInfo {
//     std::string name;
//     uint64_t imm;           // 立即数
//     uint64_t destdata;          // 目的寄存器
//     uint64_t src1data;          // 源寄存器1
//     uint64_t src2data;          // 源寄存器2
    
    
//     // 控制信号
//     bool dest_valid;        // 目的寄存器有效
//     bool alu;               // ALU操作
//     bool mem_read;          // 内存读
//     bool mem_write;         // 内存写
//     bool branch;            // 分支指令
//     bool jump;              // 跳转指令
    
//     // 初始化
//     void reset() {
//         name = "UNKNOWN";
//         imm = 0;
//         dest = 0;
//         src1 = 0;
//         src2 = 0;
//         funct3 = 0;
//         funct7 = 0;
//         opcode = 0;
//         dest_valid = false;
//         alu = false;
//         mem_read = false;
//         mem_write = false;
//         branch = false;
//         jump = false;
//     }
// };

std::ostream&
operator<<(std::ostream& os, const DynInst& di)
{
    return os << di.getMnemonic();
}

class Decode {
public:
    TimeBuffer<FetchStruct>& input;
    TimeBuffer<FetchStruct> output; // 简化：直接透传

    Decode(TimeBuffer<FetchStruct>& in) : input(in), output(0,1) {}

    void tick() {
        auto cur = input.zero();
        if ((*cur).valid) {
            std::cout << "  -> Decode: " << (*cur).inst << "\n";
            *output.getWire(1) = *cur;
        } else {
            *output.getWire(1) = FetchStruct{};
        }
    }
};


class Execute{
public:
    TimeBuffer<FetchStruct>& input;
    TimeBuffer<ControlSignals>& ctrlBuf;

    Execute(TimeBuffer<FetchStruct>& in, TimeBuffer<ControlSignals>& ctrl)
        : input(in), ctrlBuf(ctrl) {}

    void tick() {
        auto cur = input.zero();
        if ((*cur).valid) {
            std::cout << "    -> Execute: " << (*cur).inst << "\n";

            // 模拟：如果是指令 "JMP BAD"，触发 flush
            //const char* mnem = (*cur).inst->getMnemonic();
            //if (strcmp(mnem, "JMP BAD") == 0) {
            if ((*cur).inst->getMnemonic() == "JMP BAD") {
            //if ((*cur).inst == "JMP BAD") {
                ControlSignals cs;
                cs.flush_fetch = true;
                cs.redirect_pc = 2000;
                cs.reason = "Bad branch predicted";
                // 写入当前周期的控制信号（将在未来被 Fetch 读取）
                *ctrlBuf.zero() = cs;
                std::cout << "      !! Triggering flush to PC=2000 !!\n";
            }
        }
    }
};

class Commit {
public:
    TimeBuffer<FetchStruct>& input;
    TimeBuffer<ControlSignals>& ctrlBuf;

    Commit(TimeBuffer<FetchStruct>& in, TimeBuffer<ControlSignals>& ctrl)
        : input(in), ctrlBuf(ctrl) {}

    void tick() {
        auto cur = input.zero();
        if ((*cur).valid) {
            std::cout << "      -> Commit: " << *(*cur).inst << "\n";
            // 正常提交不产生 flush
            *ctrlBuf.zero() = ControlSignals{}; // 显式清空
        }
    }
};