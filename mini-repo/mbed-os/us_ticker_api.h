#pragma once
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// Real signature per mbed-os; body just needs to return *something* monotonic
// enough for compile-time and light logic checks. Trainees should not be
// relying on real timing accuracy from this mock.
inline uint32_t us_ticker_read(void) {
    static uint32_t fake_time = 0;
    fake_time += 2000; // pretend 2ms have passed each call
    return fake_time;
}

#ifdef __cplusplus
}
#endif
