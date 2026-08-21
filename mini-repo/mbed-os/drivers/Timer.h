#pragma once
#include <chrono>

namespace mbed {

class Timer {
public:
    Timer() = default;

    void start() {}
    void stop() {}
    void reset() {}

    // Placeholder: always returns zero elapsed time.
    std::chrono::microseconds elapsed_time() const {
        return std::chrono::microseconds(0);
    }
};

} // namespace mbed

using mbed::Timer;