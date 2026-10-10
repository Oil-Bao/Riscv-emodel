#ifndef __CPU_O3_COMM_HH__
#define __CPU_O3_COMM_HH__

#include <vector>
#include <list>
#include "src/base/types.hh"
#include "src/base/refcnt.hh"
#include "src/base/static_inst.hh"
#include "src/base/reg_class.hh"
#include "src/base/dyn_inst_ptr.hh"

//using DynInstPtr = RefCountingPtr<DynInst>;

/** Struct that defines the information passed from fetch to decode. */
// struct FetchStruct
// {
//     int size;

//     DynInstPtr insts[12];
//     //Fault fetchFault;
//     InstSeqNum fetchFaultSN;
//     bool clearFetchFault;
// };

struct FetchStruct {
    DynInstPtr inst;
    unsigned pc = 0;
    bool valid = false;
};

struct ControlSignals {
    bool flush_fetch = false;      // Commit/Execute 可要求 Fetch flush
    unsigned redirect_pc = 0;      // 重定向目标 PC
    std::string reason;            // flush 原因（调试用）
};

/** Struct that defines the information passed from decode to rename. */
struct DecodeStruct
{
    int size;

    DynInstPtr insts[12];
};



#endif //__CPU_O3_COMM_HH__
