#pragma once
// Compile-only stand-in for mbed's target-specific PinNames.h.
// Real mbed encodes pin identity (port + bit) into these values for the HAL;
// for a host compile check we only need *some* distinct value per name so
// code that references PA_7, PB_4, etc. resolves without pulling in a real
// STM32 target definition.

using PinName = int;

#define MBED_MOCK_PIN(port, n) ((port) * 16 + (n))

// Add more ports/pins here only if a trainee file references a name that
// isn't listed yet -- the compiler error will tell you exactly which one.
constexpr PinName PA_0  = MBED_MOCK_PIN(0, 0),  PA_1  = MBED_MOCK_PIN(0, 1),
                   PA_2  = MBED_MOCK_PIN(0, 2),  PA_3  = MBED_MOCK_PIN(0, 3),
                   PA_4  = MBED_MOCK_PIN(0, 4),  PA_5  = MBED_MOCK_PIN(0, 5),
                   PA_6  = MBED_MOCK_PIN(0, 6),  PA_7  = MBED_MOCK_PIN(0, 7),
                   PA_8  = MBED_MOCK_PIN(0, 8),  PA_9  = MBED_MOCK_PIN(0, 9),
                   PA_10 = MBED_MOCK_PIN(0, 10), PA_11 = MBED_MOCK_PIN(0, 11),
                   PA_12 = MBED_MOCK_PIN(0, 12), PA_13 = MBED_MOCK_PIN(0, 13),
                   PA_14 = MBED_MOCK_PIN(0, 14), PA_15 = MBED_MOCK_PIN(0, 15);

constexpr PinName PB_0  = MBED_MOCK_PIN(1, 0),  PB_1  = MBED_MOCK_PIN(1, 1),
                   PB_2  = MBED_MOCK_PIN(1, 2),  PB_3  = MBED_MOCK_PIN(1, 3),
                   PB_4  = MBED_MOCK_PIN(1, 4),  PB_5  = MBED_MOCK_PIN(1, 5),
                   PB_6  = MBED_MOCK_PIN(1, 6),  PB_7  = MBED_MOCK_PIN(1, 7),
                   PB_8  = MBED_MOCK_PIN(1, 8),  PB_9  = MBED_MOCK_PIN(1, 9),
                   PB_10 = MBED_MOCK_PIN(1, 10), PB_11 = MBED_MOCK_PIN(1, 11),
                   PB_12 = MBED_MOCK_PIN(1, 12), PB_13 = MBED_MOCK_PIN(1, 13),
                   PB_14 = MBED_MOCK_PIN(1, 14), PB_15 = MBED_MOCK_PIN(1, 15);

constexpr PinName PC_0  = MBED_MOCK_PIN(2, 0),  PC_1  = MBED_MOCK_PIN(2, 1),
                   PC_2  = MBED_MOCK_PIN(2, 2),  PC_3  = MBED_MOCK_PIN(2, 3),
                   PC_4  = MBED_MOCK_PIN(2, 4),  PC_5  = MBED_MOCK_PIN(2, 5),
                   PC_6  = MBED_MOCK_PIN(2, 6),  PC_7  = MBED_MOCK_PIN(2, 7),
                   PC_8  = MBED_MOCK_PIN(2, 8),  PC_9  = MBED_MOCK_PIN(2, 9),
                   PC_10 = MBED_MOCK_PIN(2, 10), PC_11 = MBED_MOCK_PIN(2, 11),
                   PC_12 = MBED_MOCK_PIN(2, 12), PC_13 = MBED_MOCK_PIN(2, 13),
                   PC_14 = MBED_MOCK_PIN(2, 14), PC_15 = MBED_MOCK_PIN(2, 15);

constexpr PinName PD_0  = MBED_MOCK_PIN(3, 0),  PD_1  = MBED_MOCK_PIN(3, 1),
                   PD_2  = MBED_MOCK_PIN(3, 2),  PD_3  = MBED_MOCK_PIN(3, 3),
                   PD_4  = MBED_MOCK_PIN(3, 4),  PD_5  = MBED_MOCK_PIN(3, 5),
                   PD_6  = MBED_MOCK_PIN(3, 6),  PD_7  = MBED_MOCK_PIN(3, 7),
                   PD_8  = MBED_MOCK_PIN(3, 8),  PD_9  = MBED_MOCK_PIN(3, 9),
                   PD_10 = MBED_MOCK_PIN(3, 10), PD_11 = MBED_MOCK_PIN(3, 11),
                   PD_12 = MBED_MOCK_PIN(3, 12), PD_13 = MBED_MOCK_PIN(3, 13),
                   PD_14 = MBED_MOCK_PIN(3, 14), PD_15 = MBED_MOCK_PIN(3, 15);

constexpr PinName NC = -1; // "not connected", mbed's convention

enum class PinMode {
    PullNone,
    PullUp,
    PullDown,
    OpenDrain,
    OpenDrainPullUp,
    PullDefault
};

inline void pin_mode(PinName, PinMode) { /* no-op: no real GPIO on host */ }
