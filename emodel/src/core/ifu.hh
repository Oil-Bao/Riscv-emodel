
#include "src/base/types.hh"
#include "src/base/timebuf.hh"
#include "src/base/common.hh"


class BaseCPUParams;

class CPU;

class Fetch
{
  public:
    enum ThreadStatus
    {
        Running,
        Idle,
        Squashing,
        Blocked,
        Fetching,
        TrapPending,
        QuiescePending,
        ItlbWait,
        IcacheWaitResponse,
        IcacheWaitRetry,
        IcacheAccessComplete,
        NoGoodAddr
    };

    public:
    /** Fetch constructor. */
        Fetch(const BaseCPUParams &params,TimeBuffer<ControlSignals>& ctrl);
        TimeBuffer<FetchStruct> output;
        TimeBuffer<ControlSignals>& ctrlBuf;
        TimeBuffer<ControlSignals>::wire fromCommit; // Commit -> Fetch 延迟 3 周期

        void tick();
    private:
    /** The width of fetch in instructions. */
    unsigned FetchWidth;

    /** The width of decode in instructions. */
    unsigned DecodeWidth;
    Addr BootPC;

    Addr pc;
    int cycle;
    bool stalled;
    const std::vector<std::string> program = {"ADD", "SUB", "JMP BAD", "MUL", "HALT"};

    static constexpr int commitToFetchDelay = 3;


};