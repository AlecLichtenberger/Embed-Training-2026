#pragma once
#include <chrono>
#include <cstdint>

namespace rtos {
namespace ThisThread {

template <typename Rep, typename Period>
inline void sleep_for(std::chrono::duration<Rep, Period>) {
    // placeholder — no-op in this mock environment
}

inline void sleep_for(uint32_t millisec) { (void)millisec; }

} // namespace ThisThread
} // namespace rtos

using namespace std::chrono_literals; // enables e.g. 10ms, 500us literals