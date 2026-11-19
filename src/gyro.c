#include "gyro.h"
#include <math.h>

// gyro_init: Initializes i2c port gyro
void gyro_init(i2c_inst_t *i2c_port) {
    // Turn on gyro from powerdown
    uint8_t ctrl1[] = {i2c_port, 0x57};
    i2c_write_blocking(i2c_port, GYRO_ADDR, ctrl1, 2, false);

    // Set continuous standard mode
    uint8_t ctrl4[] = {REG_CTRL_4, 0x00};
    i2c_write_blocking(i2c_port, GYRO_ADDR, ctrl4, 2, false);
}

//gyro_read_raw: Gets x,y,z acceleromter data from gyroscope.
//Outputs updated data to accel[], function returns true.
//If data is not read/wrote, then function returns false.

int gyro_read_raw(i2c_inst_t *i2c_port, int16_t accel[3]) {
    //auto increment request by or 0x80 to msb
    uint8_t reg = REG_OUT | 0x80; 
    uint8_t data[6];

    int write = i2c_write_blocking(i2c_port, GYRO_ADDR, &reg, 1, true);
    if (write < 0) {
        return 0;
    }

    int read = i2c_read_blocking(i2c_port, GYRO_ADDR, data, 6, false);
    if (read < 0) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        //module stores in little endian.
        //Ors the data for high (shifting 8 for high) and low bits together
        int16_t raw = (int16_t)((data[(i * 2) + 1] << 8) | data[i * 2]);
        //then shifts back to 6 to put back into g format (ex if accel[1] = 1, it would be 1/512 of a 9.8 m/s).
        accel[i] = raw >> 6; 
    }

    return 1;
}

//uses arctan calcilation to transform data from accelerometer to pitch and roll data (in degrees)
void calculate_orientation(int16_t accel[3], float *pitch, float *roll) {
    float ax = (float)accel[0];
    float ay = (float)accel[1];
    float az = (float)accel[2];

    // Calculate Pitch and Roll
    *pitch = atan2(ay, sqrt(ax * ax + az * az)) * (180.0 / M_PI);
    *roll  = atan2(-ax, sqrt(ay * ay + az * az)) * (180.0 / M_PI);
}