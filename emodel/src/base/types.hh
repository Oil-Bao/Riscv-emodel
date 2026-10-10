#ifndef __BASE_TYPES_H__
#define __BASE_TYPES_H__

#include <inttypes.h>
#include <iomanip>
#include <cstring>
#include <cassert>

typedef uint64_t Addr;


typedef uint64_t InstSeqNum;

using RegIndex = uint16_t;
using RegVal = uint64_t;

template<typename T>
class Logic{
private:
    T data;
    
public:
    // 构造函数
    constexpr Logic(T d = 0) : data(d) {}
    
    operator T() const { return data; }
    
    constexpr T operator()(int high, int low) const {
        assert(high >= low && high < sizeof(T) && low >= 0);
        int width = high - low + 1;
        return (data >> low) & ((1u << width) - 1);
    }
     //  sign_extend()
    template<int Bits>
    constexpr int64_t sign_extend() const {
        static_assert(Bits > 0 && Bits <= 64, "Bits must be between 1 and 64");
        if constexpr (Bits == 64) {
            return static_cast<int64_t>(data);
        } else {
            uint64_t mask = 1u << (Bits - 1);
            uint64_t value = data & ((1u << Bits) - 1);
            return static_cast<int64_t>((value ^ mask) - mask);
        }
    }
};


#endif