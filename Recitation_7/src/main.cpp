#include "mbed.h"

I2C i2c(PB_11, PB_10);

#define LSM6DSL_ADDR   (0x6A << 1)
#define WHO_AM_I       0x0F
#define CTRL1_XL       0x10
#define CTRL2_G        0x11
#define CTRL3_C        0x12
#define DRDY_PULSE_CFG 0x0B
#define INT1_CTRL      0x0D
#define STATUS_REG     0x1E
#define OUTX_L_G       0x22
#define OUTX_L_XL      0x28

InterruptIn int1_pin(PD_11, PullDown);
volatile bool data_ready = false;

void data_ready_isr() {
    data_ready = true;
}

struct SensorData {
    float acc[3];
    float gyro[3];
};

volatile SensorData latest;

bool write_reg(uint8_t reg, uint8_t val) {
    char buf[2] = {(char)reg, (char)val};
    return (i2c.write(LSM6DSL_ADDR, buf, 2) == 0);
}

bool read_reg(uint8_t reg, uint8_t &val) {
    char r = (char)reg;
    if (i2c.write(LSM6DSL_ADDR, &r, 1, true) != 0) return false;
    if (i2c.read(LSM6DSL_ADDR, &r, 1) != 0) return false;
    val = (uint8_t)r;
    return true;
}

bool read_int16(uint8_t reg_low, int16_t &val) {
    uint8_t lo, hi;
    if (!read_reg(reg_low, lo)) return false;
    if (!read_reg(reg_low + 1, hi)) return false;
    val = (int16_t)((hi << 8) | lo);
    return true;
}

bool init_sensor() {
    uint8_t who;
    if (!read_reg(WHO_AM_I, who) || who != 0x6A) {
        printf("Sensor not found\r\n");
        return false;
    }

    write_reg(CTRL3_C, 0x44);
    write_reg(CTRL1_XL, 0x40);       // 104Hz, ±2g
    write_reg(CTRL2_G, 0x40);        // 104Hz, ±250 dps
    write_reg(INT1_CTRL, 0x03);
    write_reg(DRDY_PULSE_CFG, 0x80);

    ThisThread::sleep_for(100ms);

    uint8_t dummy;
    read_reg(STATUS_REG, dummy);

    int16_t temp;
    for (int i = 0; i < 6; i++) {
        read_int16(OUTX_L_XL + i * 2, temp);
    }

    int1_pin.rise(&data_ready_isr);
    return true;
}

// High priority thread   reads sensor as soon as data is ready
void acquisition_task() {
    while (true) {
        if (data_ready) {
            data_ready = false;

            int16_t acc[3], gyro[3];
            for (int i = 0; i < 3; i++) {
                read_int16(OUTX_L_XL + i * 2, acc[i]);
                read_int16(OUTX_L_G  + i * 2, gyro[i]);
            }

            // ±2g: 0.061 mg/LSB
            latest.acc[0] = acc[0] * 0.000061f;
            latest.acc[1] = acc[1] * 0.000061f;
            latest.acc[2] = acc[2] * 0.000061f;

            // ±250 dps: 8.75 mdps/LSB
            latest.gyro[0] = gyro[0] * 0.00875f;
            latest.gyro[1] = gyro[1] * 0.00875f;
            latest.gyro[2] = gyro[2] * 0.00875f;
        }
        ThisThread::sleep_for(1ms);
    }
}

// Normal priority thread   prints at a comfortable rate
void print_task() {
    while (true) {
        printf(">acc_x:%.3f\n>acc_y:%.3f\n>acc_z:%.3f\n"
               ">gyro_x:%.2f\n>gyro_y:%.2f\n>gyro_z:%.2f\n",
               latest.acc[0], latest.acc[1], latest.acc[2],
               latest.gyro[0], latest.gyro[1], latest.gyro[2]);

        ThisThread::sleep_for(100ms);
    }
}

int main() {
    static BufferedSerial pc(USBTX, USBRX, 115200);
    i2c.frequency(400000);

    if (!init_sensor()) {
        while (true) { ThisThread::sleep_for(1s); }
    }

    Thread acq_thread(osPriorityHigh);
    Thread print_thread(osPriorityNormal);

    acq_thread.start(acquisition_task);
    print_thread.start(print_task);

    while (true) {
        ThisThread::sleep_for(1s);
    }
}