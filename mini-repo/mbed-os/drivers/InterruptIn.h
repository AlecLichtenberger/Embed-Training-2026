#pragma once
#include "platform/Callback.h"
#include "PinNames.h"

namespace mbed {

class InterruptIn {
public:
    explicit InterruptIn(PinName pin) : _pin(pin) {}

    void rise(Callback<void()> cb) { _rise_cb = cb; }
    void fall(Callback<void()> cb) { _fall_cb = cb; }
    void mode(int pull_mode) { (void)pull_mode; }
    void enable_irq() {}
    void disable_irq() {}

    int read() const { return 0; }
    operator int() const { return read(); }

private:
    PinName _pin;
    Callback<void()> _rise_cb;
    Callback<void()> _fall_cb;
};

} // namespace mbed

using mbed::InterruptIn;