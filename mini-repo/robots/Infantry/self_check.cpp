// Proves the mock mbed.h satisfies the exact patterns used in Infantry.cpp / ISM330.cpp
#include "mbed.h"
#include <pinmap.h>
#include <us_ticker_api.h>
#include <us_ticker_defines.h>
#include <cstdio>

constexpr auto IMU_I2C_SDA = PB_7;
constexpr auto IMU_I2C_SCL = PB_8;
constexpr auto IMU_RESET   = PA_8;

class FakeIMUDriver {
public:
    FakeIMUDriver(I2C& i2c, uint8_t address) : i2c(i2c), _address(address) {}

    bool begin() {
        i2c.frequency(400000);
        char cmd[2] = {0x12, 0x01};
        i2c.write(_address, cmd, 2, false);
        ThisThread::sleep_for(100ms);
        return true;
    }

private:
    I2C& i2c;
    uint8_t _address;
};

class Infantry {
public:
    I2C i2c_;
    BufferedSerial jetson_raw_serial;
    FakeIMUDriver imu_;

    Infantry()
        : i2c_(IMU_I2C_SDA, IMU_I2C_SCL),
          jetson_raw_serial(PC_12, PD_2, 115200),
          imu_(i2c_, 0x6B)
    {
        pin_mode(IMU_I2C_SCL, PinMode::OpenDrainPullUp);
        pin_mode(IMU_I2C_SDA, PinMode::OpenDrainPullUp);
    }

    void init() {
        unsigned long timer = us_ticker_read();
        imu_.begin();
        printf("timer=%lu\n", timer);
    }
};

int main() {
    Infantry robot;
    robot.init();
    printf("mock mbed smoke test: compiled and ran OK\n");
    return 0;
}
