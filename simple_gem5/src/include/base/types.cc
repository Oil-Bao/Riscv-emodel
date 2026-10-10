#include "types.hh"

namespace gem5
{

std::ostream&
operator<<(std::ostream &out, const Cycles & cycles)
{
    out << cycles.c;
    return out;
}

} // namespace gem5
