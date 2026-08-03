#pragma once
// Compile-only stand-in for mbed's CAN.h. Same rule as the rest of
// mock_mbed/: signatures match real mbed-os, bodies are harmless no-ops.
// Extend this only when a real compiler error names something missing here.

#include <cstring>
#include "PinNames.h"

// Real mbed asserts fire (and can halt) in debug builds; a no-op is fine
// here since this scaffold only checks that code type-checks, not that it
// behaves correctly at runtime.
#ifndef MBED_ASSERT
#define MBED_ASSERT(expr) ((void)0)
#endif

namespace mbed {

enum CANFormat { CANStandard = 0, CANExtended = 1, CANAny = 2 };
enum CANType   { CANData = 0, CANRemote = 1 };
enum Mode      { Reset, Normal, Silent, LocalTest, GlobalTest, SilentTest };

class CANMessage {
public:
    unsigned int id;
    CANFormat format;
    CANType type;
    unsigned char len;
    unsigned char data[8];

    CANMessage() : id(0), format(CANStandard), type(CANData), len(0), data{} {}

    CANMessage(int _id, const char* _data, char _len = 8,
               CANType _type = CANData, CANFormat _format = CANStandard)
        : id(_id), format(_format), type(_type), len(_len)
    {
        for (int i = 0; i < 8; i++) {
            data[i] = (_data && i < _len) ? static_cast<unsigned char>(_data[i]) : 0;
        }
    }

    CANMessage(int _id, CANFormat _format = CANStandard)
        : id(_id), format(_format), type(CANRemote), len(0), data{} {}
};

// Minimal stand-in for mbed::Callback<void()> so .attach(...) compiles
// with a plain function pointer, lambda, or (obj, &Class::method).
template <typename Signature>
class Callback;

template <typename R>
class Callback<R()> {
public:
    Callback() = default;
    template <typename F>
    Callback(F) {}
    template <typename T>
    Callback(T*, R (T::*)()) {}
    R call() { return R(); }
};

class CAN {
public:
    // Real mbed nests this on CAN itself -- accessed as CAN::IrqType::RxIrq
    enum IrqType { RxIrq, TxIrq, EwIrq, DoIrq, WuIrq, EpIrq, AlIrq, BeIrq, IdIrq };

    CAN(PinName /*rd*/, PinName /*td*/) {}
    CAN(PinName /*rd*/, PinName /*td*/, int /*hz*/) {}

    int frequency(int) { return 1; }
    int write(CANMessage) { return 1; }
    int read(CANMessage&, int /*handle*/ = 0) { return 0; }
    void reset() {}
    int mode(Mode) { return 0; }
    int filter(unsigned int /*id*/, unsigned int /*mask*/,
               CANFormat /*format*/ = CANAny, int /*handle*/ = 0) { return 0; }
    unsigned char rderror() { return 0; }
    unsigned char tderror() { return 0; }

    void attach(Callback<void()> /*func*/, IrqType /*type*/ = RxIrq) {}
    template <typename T>
    void attach(T* obj, void (T::*method)(), IrqType type = RxIrq) {}
};

} // namespace mbed

using namespace mbed;
