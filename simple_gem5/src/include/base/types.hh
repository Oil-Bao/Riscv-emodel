#ifndef __BASE_TYPES_HH__
#define __BASE_TYPES_HH__

#include <inttypes.h>
#include <map>
#include <cassert>
#include <limits>
#include <memory>
#include <ostream>
#include <stdexcept>

namespace gem5
{

typedef int16_t PortID;
typedef int64_t Counter;
typedef uint64_t Addr;
typedef uint64_t Tick;
const Tick MaxTick = 0xffffffffffffffffULL;
typedef int ContextID;
const ContextID InvalidContextID = (ContextID)-1;
typedef uint64_t InstSeqNum;
const Addr MaxAddr = (Addr)-1;

const PortID InvalidPortID = (PortID)-1;
typedef unsigned int off_type;
typedef int16_t ThreadID;
/// The universal simulation clock.

class Cycles
{
  private:
    /** Member holding the actual value. */
    uint64_t c;
  public:
    /** Explicit constructor assigning a value. */
    explicit constexpr Cycles(uint64_t _c) : c(_c) { }
    /** Default constructor for parameter classes. */
    Cycles() : c(0) { }

    /** Converting back to the value type. */
    constexpr operator uint64_t() const { return c; }

    /** Prefix increment operator. */
    Cycles& operator++() { ++c; return *this; }

    /** Prefix decrement operator. Is only temporarily used in the O3 CPU. */
    Cycles& operator--() { assert(c != 0); --c; return *this; }

    /** In-place addition of cycles. */
    Cycles& operator+=(const Cycles& cc) { c += cc.c; return *this; }

    /** Greater than comparison used for > Cycles(0). */
    constexpr bool
    operator>(const Cycles& cc) const
    {
        return c > cc.c;
    }

    constexpr Cycles
    operator+(const Cycles& b) const
    {
        return Cycles(c + b.c);
    }

    constexpr Cycles
    operator-(const Cycles& b) const
    {
        return c >= b.c ? Cycles(c - b.c) :
            throw std::invalid_argument("RHS cycle value larger than LHS");
    }

    constexpr Cycles
    operator <<(const int32_t shift) const
    {
        return Cycles(c << shift);
    }

    constexpr Cycles
    operator >>(const int32_t shift) const
    {
        return Cycles(c >> shift);
    }

    friend std::ostream& operator<<(std::ostream &out, const Cycles & cycles);
};

class FaultBase;
typedef std::shared_ptr<FaultBase> Fault;

typedef uint16_t MicroPC;
typedef double Result;
//typedef std::vector<Result> VResult;
//typedef std::vector<Counter> VCounter;

using RegVal = uint64_t;
using RegIndex = uint16_t;

enum DistType { Deviation, Dist, Hist };

/** General container for distribution data. */
struct DistData
{
    DistType type;
    Counter min;
    Counter max;
    Counter bucket_size;

    Counter min_val;
    Counter max_val;
    Counter underflow;
    Counter overflow;
    
    Counter sum;
    Counter squares;
    Counter logs;
    Counter samples;
};

typedef unsigned int size_type;
typedef std::map<Counter, int> MCounter;
/** Data structure of sparse histogram */
struct SparseHistData
{
    MCounter cmap;
    Counter samples;
};

typedef std::numeric_limits<Counter> CounterLimits;
// Rather than creating a shared_ptr instance and assigning it nullptr,
// we just create an alias.
constexpr decltype(nullptr) NoFault = nullptr;

} // namespace gem5

#endif // __BASE_TYPES_HH__