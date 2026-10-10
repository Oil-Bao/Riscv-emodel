#ifndef __CPU_STATIC_INST_HH__
#define __CPU_STATIC_INST_HH__

#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <string>
#include "src/base/refcnt.hh"
#include "src/base/inst_enums.hh"


struct StaticInstFlags;

class StaticInst : public RefCounted, public StaticInstFlags
{
//   public:
//     using RegIdArrayPtr = RegId (StaticInst:: *)[];

//   private:
//     /// See srcRegIdx().
//     RegIdArrayPtr _srcRegIdxPtr = nullptr;

//     /// See destRegIdx().
//     RegIdArrayPtr _destRegIdxPtr = nullptr;

  protected:

    /// Flag values for this instruction.
    std::bitset<Num_Flags> flags;

    /// See opClass().
    OpClass _opClass;

    /// See numSrcRegs().
    uint8_t _numSrcRegs = 0;

    /// See numDestRegs().
    uint8_t _numDestRegs = 0;

 //   std::array<uint8_t, MiscRegClass + 1> _numTypedDestRegs = {};

  public:

    /// @name Register information.
    /// The sum of the different numDestRegs([type])-s equals numDestRegs().
    /// The per-type function is used to track physical register usage.
    //@{
    /// Number of source registers.
    uint8_t numSrcRegs()  const { return _numSrcRegs; }
    /// Number of destination registers.
    uint8_t numDestRegs() const { return _numDestRegs; }
    /// Number of destination registers of a particular type.
    // uint8_t
    // numDestRegs(RegClassType type) const
    // {
    //     return _numTypedDestRegs[type];
    // }
    //@}

    /// @name Flag accessors.
    /// These functions are used to access the values of the various
    /// instruction property flags.  See StaticInst::Flags for descriptions
    /// of the individual flags.
    //@{

    bool isNop()          const { return flags[IsNop]; }

    bool
    isMemRef() const
    {
        return flags[IsLoad] || flags[IsStore] || flags[IsAtomic];
    }
    bool isLoad()         const { return flags[IsLoad]; }
    bool isStore()        const { return flags[IsStore]; }
    bool isAtomic()       const { return flags[IsAtomic]; }
    bool isStoreConditional()     const { return flags[IsStoreConditional]; }
    bool isInstPrefetch() const { return flags[IsInstPrefetch]; }
    bool isDataPrefetch() const { return flags[IsDataPrefetch]; }
    bool isPrefetch()     const { return isInstPrefetch() ||
                                         isDataPrefetch(); }

    bool isInteger()      const { return flags[IsInteger]; }
    bool isFloating()     const { return flags[IsFloating]; }
    bool isVector()       const { return flags[IsVector]; }
    bool isMatrix()       const { return flags[IsMatrix]; }

    bool isControl()      const { return flags[IsControl]; }
    bool isCall()         const { return flags[IsCall]; }
    bool isReturn()       const { return flags[IsReturn]; }
    bool isDirectCtrl()   const { return flags[IsDirectControl]; }
    bool isIndirectCtrl() const { return flags[IsIndirectControl]; }
    bool isCondCtrl()     const { return flags[IsCondControl]; }
    bool isUncondCtrl()   const { return flags[IsUncondControl]; }

    bool isSerializing()  const { return flags[IsSerializing] ||
                                      flags[IsSerializeBefore] ||
                                      flags[IsSerializeAfter]; }
    bool isSerializeBefore() const { return flags[IsSerializeBefore]; }
    bool isSerializeAfter() const { return flags[IsSerializeAfter]; }
    bool isSquashAfter() const { return flags[IsSquashAfter]; }
    bool
    isFullMemBarrier() const
    {
        return flags[IsReadBarrier] && flags[IsWriteBarrier];
    }
    bool isReadBarrier() const { return flags[IsReadBarrier]; }
    bool isWriteBarrier() const { return flags[IsWriteBarrier]; }
    bool isNonSpeculative() const { return flags[IsNonSpeculative]; }
    bool isQuiesce() const { return flags[IsQuiesce]; }
    bool isUnverifiable() const { return flags[IsUnverifiable]; }
    bool isPseudo() const { return flags[IsPseudo]; }
    bool isSyscall() const { return flags[IsSyscall]; }
    bool isMacroop() const { return flags[IsMacroop]; }
    bool isMicroop() const { return flags[IsMicroop]; }
    bool isDelayedCommit() const { return flags[IsDelayedCommit]; }
    bool isLastMicroop() const { return flags[IsLastMicroop]; }
    bool isFirstMicroop() const { return flags[IsFirstMicroop]; }
    // hardware transactional memory
    // HtmCmds must be identified as such in order
    // to provide them with necessary memory ordering semantics.
    bool isHtmStart() const { return flags[IsHtmStart]; }
    bool isHtmStop() const { return flags[IsHtmStop]; }
    bool isHtmCancel() const { return flags[IsHtmCancel]; }

    bool isInvalid() const { return flags[IsInvalid]; }

    bool
    isHtmCmd() const
    {
        return isHtmStart() || isHtmStop() || isHtmCancel();
    }
    //@}

    void setFirstMicroop() { flags[IsFirstMicroop] = true; }
    void setLastMicroop() { flags[IsLastMicroop] = true; }
    void setDelayedCommit() { flags[IsDelayedCommit] = true; }
    void setFlag(Flags f) { flags[f] = true; }

    /// Operation class.  Used to select appropriate function unit in issue.
    OpClass opClass() const { return _opClass; }


    /// Return logical index (architectural reg num) of i'th destination reg.
    /// Only the entries from 0 through numDestRegs()-1 are valid.
    // const RegId &destRegIdx(int i) const { return (this->*_destRegIdxPtr)[i]; }

    // void
    // setDestRegIdx(int i, const RegId &val)
    // {
    //     (this->*_destRegIdxPtr)[i] = val;
    // }

    // /// Return logical index (architectural reg num) of i'th source reg.
    // /// Only the entries from 0 through numSrcRegs()-1 are valid.
    // const RegId &srcRegIdx(int i) const { return (this->*_srcRegIdxPtr)[i]; }

    // void
    // setSrcRegIdx(int i, const RegId &val)
    // {
    //     (this->*_srcRegIdxPtr)[i] = val;
    // }

    // /// Pointer to a statically allocated "null" instruction object.
    // static StaticInstPtr nullStaticInstPtr;


//   protected:

//     /**
//      * Set the pointers which point to the arrays of source and destination
//      * register indices. These will be defined in derived classes which know
//      * what size they need to be, and installed here so they can be accessed
//      * with the base class accessors.
//      */
//     void
//     setRegIdxArrays(RegIdArrayPtr src, RegIdArrayPtr dest)
//     {
//         _srcRegIdxPtr = src;
//         _destRegIdxPtr = dest;
//     }

    const char *mnemonic;
    /// Constructor.
    /// It's important to initialize everything here to a sane
    /// default, since the decoder generally only overrides
    /// the fields that are meaningful for the particular
    /// instruction.
    StaticInst(const char *_mnemonic, OpClass op_class)
        : _opClass(op_class), mnemonic(_mnemonic)
    {}

  public:
    virtual ~StaticInst() {};
};


#endif // __CPU_STATIC_INST_HH__
