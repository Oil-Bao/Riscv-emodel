

class BaseCPUParams;

class CPU{
public:
    enum Status
    {
        Running,
        Idle,
        Halted,
        Blocked,
        SwitchedOut
    };
public:
    /** Constructs a CPU with the given parameters. */
    CPU(const BaseCPUParams &params);
};