#include "BNO055.h"


/*
We added these two functions since they're definitely beyond what we expect from you as recruits
I'd be impressed if you understood the math behind them.
Basically just know that quats are 4 axis (1 real, 3 imaginary) numbers that allow us 
to nicely talk about rotation, and the functions spit out pitch roll and yaw. 
*/
void BNO055::get_quaternion(BNO055_QUATERNION_TypeDef *result)
{
    if (cantReadDataCount > 0 && cantReadDataCount < 50) {
        cantReadDataCount++;
        return;
    } else if (cantReadDataCount >= 50) {
        cantReadDataCount = 1;
    }
    int16_t w,x,y,z;

    dt[0] = BNO055_QUATERNION_W_LSB;
    int writeResult = _i2c.write(chip_addr, dt, 1, true);
    if (!writeResult)  {
        if (cantReadDataCount > 0) {
            printf("RESET IMU\n");
            reset();
            cantReadDataCount = 0;
        }
        _i2c.read(chip_addr, dt, 8, false);
        w = dt[1] << 8 | dt[0];
        x = dt[3] << 8 | dt[2];
        y = dt[5] << 8 | dt[4];
        z = dt[7] << 8 | dt[6];

        result->w = (double)w / 16384.0f;
        result->x = (double)x / 16384.0f;
        result->y = (double)y / 16384.0f;
        result->z = (double)z / 16384.0f;
    } else {
        cantReadDataCount++;
    }
}

void BNO055::get_angular_position_quat(IMU::EulerAngles *result){

    BNO055_QUATERNION_TypeDef q;
    get_quaternion(&q);

    float roll  = atan2(2 * (q.w * q.x + q.y * q.z), 1 - 2 * (q.x * q.x + q.y * q.y)) * 180 / PI;
    float pitch = asin(2 * q.w * q.y - q.x * q.z) * 180 / PI;
    float yaw   = atan2(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (q.y * q.y + q.z * q.z)) * 180 / PI;

    memcpy(&result->roll, &roll, sizeof(float));
    memcpy(&result->pitch, &pitch, sizeof(float));
    memcpy(&result->yaw, &yaw, sizeof(float));
}


//Constructors
BNO055::BNO055(I2C &i2c, uint8_t addr, PinName p_reset) noexcept : _i2c(i2c), chip_addr(addr), _res(p_reset){ //Not sure if this extra parameter is inited properly?
    IMU::EulerAngles imuAngles;
    cantReadDataCount = 0;
    _i2c_p = &i2c;
    BNO055_VECTOR_TypeDef imuVector;
} 

//Function Implementations

/**
 * Initialize the BNO055
 */
void BNO055::init() noexcept{
    _i2c.frequency(100); //Magic number of the recommended 100khz for now
}

/**
 * Reset the BNO055
 */
void BNO055::reset() noexcept{

}

/**
 * Get acceleration of the BNO055, through I2C
 * then parse and pass it to the struct
 * @param la pointer to the x,y,z of the imu
 */
void BNO055::get_accel(BNO055_VECTOR_TypeDef *la){
    if (cantReadDataCount > 0 && cantReadDataCount < 50) {
        cantReadDataCount++;
        return;
    } else if (cantReadDataCount >= 50) {
        cantReadDataCount = 1;
    }
    char writeArr [1] = {0x08};
    int writeResult = _i2c.write(chip_addr, writeArr, 1, false);
    
    if (!writeResult)  {
        if (cantReadDataCount > 0) {
            printf("RESET IMU\n");
            reset();
            cantReadDataCount = 0;
        }
        _i2c.read(chip_addr, dt, 6, true); // Reads the contents of the accel registers into dt
        
        //Move contents of dt into the Vector Type Def struct via the given pointer to the struct
        int x_LSB = dt[0];
        int x_MSB = dt[1];
        int y_LSB = dt[2];
        int y_MSB = dt[3];
        int z_LSB = dt[4];
        int z_MSB = dt[5];
        
        //bit shifting
        uint16_t accel_X = (x_MSB << 8) | x_LSB;
        uint16_t accel_Y = (y_MSB << 8) | y_LSB;
        uint16_t accel_Z = (z_MSB << 8) | z_LSB;

        //Load into the passed struct
        la-> x = accel_X;
        la-> y = accel_Y;
        la-> z = accel_Z;
        cantReadDataCount++;
    }
}

/**
 * Get gyro of BNO055 using I2C comms and then pass to the struct 
 * after parsing
 * @param gr pointer to the x,y,z of the imu
 */
void BNO055::get_gyro(BNO055_VECTOR_TypeDef *gr){
    char writeArr [1] = {0x0E};
    int writeResult = _i2c.write(chip_addr, writeArr, 1, false);
    _i2c.read(chip_addr, dt, 6, true); // Reads the contents of the accel registers into dt

    //Move contents of dt into the Vector Type Def struct via the given pointer to the struct
    int x_LSB = dt[0];
    int x_MSB = dt[1];
    int y_LSB = dt[2];
    int y_MSB = dt[3];
    int z_LSB = dt[4];
    int z_MSB = dt[5];

    //bit shifting
    uint16_t gyro_X = (x_MSB << 8) | x_LSB;
    uint16_t gyro_Y = (y_MSB << 8) | y_LSB;
    uint16_t gyro_Z = (z_MSB << 8) | z_LSB;

    //Load into the passed struct
    gr-> x = gyro_X;
    gr-> y = gyro_Y;
    gr-> z = gyro_Z;
}

/** Change to 9DOF fusion mode
* @param mode fusion mode
* @return none
*/
void BNO055::change_fusion_mode(uint8_t mode){

}

IMU::EulerAngles BNO055::read(){

}

IMU::EulerAngles BNO055::getImuAngles(){

}


