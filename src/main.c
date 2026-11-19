#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "gyro.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

int main() {
    stdio_init_all();

    //Initialize i2C, set gpio mode and initialize gyroscope
    i2c_init(I2C_PORT, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    sleep_ms(500); //added delay for power stability
    gyro_init(I2C_PORT);

    int16_t accel_raw[3];
    float pitch, roll;

    //basic method to read gyro data - loop gyro read, update accel_raw, and calculate data
    //roll will be most useful data for this project (to get rotation board data)
    for (;;) {
        if (!gyro_read_raw(I2C_PORT, accel_raw)) {
            sleep_ms(100);
            continue;
        }

        calculate_orientation(accel_raw, &pitch, &roll);

        // Print Pitch and Roll
        printf("Pitch: %.2f° | Roll: %.2f° \n", pitch, roll);

        sleep_ms(100);
    }
}