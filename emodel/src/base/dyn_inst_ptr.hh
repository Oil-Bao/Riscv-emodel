#ifndef __CPU_O3_DYN_INST_PTR_HH__
#define __CPU_O3_DYN_INST_PTR_HH__

#include "src/base/refcnt.hh"

class DynInst;

using DynInstPtr = RefCountingPtr<DynInst>;
using DynInstConstPtr = RefCountingPtr<const DynInst>;


#endif // __CPU_O3_DYN_INST_PTR_HH__
