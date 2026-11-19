#ifndef GYRO_H
#define GYRO_H

#include <stdint.h>
#include "hardware/i2c.h"

// Configuration constants
#define GYRO_ADDR 0x18
#define REG_CTRL_1 0x20
#define REG_CTRL_4 0x23
#define REG_OUT 0x28

// Function prototypes
void gyro_init(i2c_inst_t *i2c_port);
int gyro_read_raw(i2c_inst_t *i2c_port, int16_t accel[3]);
void calculate_orientation(int16_t accel[3], float *pitch, float *roll);

#endif