#ifndef __DYN_INST_HH__
#define __DYN_INST_HH__

#include "src/base/refcnt.hh"
#include "src/base/types.hh"
#include "src/base/dyn_inst_ptr.hh"
#include "src/base/static_inst_fwd.hh"
#include "src/base/reg_class.hh"
#include "src/base/static_inst.hh"
#include <list>


class RefCountedBase {
protected:
    mutable int refCount = 0;

public:
    void incref() const { ++refCount; }
    void decref() const {
        if (--refCount == 0)
            delete this;
    }
    int getRefCount() const { return refCount; }
};

class DynInst : public RefCountedBase {
    StaticInstPtr staticInst;
    InstSeqNum seqNum;

    // 私有构造函数 —— 只允许通过工厂创建
    DynInst(const StaticInstPtr& si, InstSeqNum sn) : staticInst(si), seqNum(sn) {}

public:
    typedef typename std::list<DynInstPtr>::iterator ListIt;

    struct Arrays
    {
        size_t numSrcs;
        size_t numDests;

        RegId *flatDestIdx;
        PhysRegIdPtr *destIdx;
        PhysRegIdPtr *prevDestIdx;
        PhysRegIdPtr *srcIdx;
        uint8_t *readySrcIdx;
    };

    static DynInstPtr create(const StaticInstPtr& si, InstSeqNum sn) {
        return new DynInst(si, sn); // RefCountingPtr 构造时会调用 incref()
    }

    //const std::string& getMnemonic() const { return staticInst ? std::string(staticInst->mnemonic) : "" }
    std::string getMnemonic() const {
        return staticInst ? staticInst->mnemonic : "";
    }
    InstSeqNum getSeqNum() const { return seqNum; }
};

#endif //__DYN_INST_HH__