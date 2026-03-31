#include "mbed.h"

I2C i2c(PB_11, PB_10);

#define LSM6DSL_ADDR (0x6A << 1)
#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define DRDY_PULSE_CFG 0x0B
#define INT1_CTRL 0x0D
#define STATUS_REG 0x1E
#define OUTX_L_G 0x22
#define OUTX_L_XL 0x28

// Hardware interrupt on INT1 pin   fires when sensor has new data
InterruptIn int1(PD_11, PullDown);

// Flag set by ISR   must be volatile so compiler does not optimize it away
volatile bool data_ready = false;

// ISR   keep it tiny, just set the flag
void data_ready_isr() { data_ready = true; }

bool write_reg(uint8_t reg, uint8_t val) {
  char buf[2] = {(char)reg, (char)val};
  return (i2c.write(LSM6DSL_ADDR, buf, 2) == 0);
}

bool read_reg(uint8_t reg, uint8_t &val) {
  char r = (char)reg;
  if (i2c.write(LSM6DSL_ADDR, &r, 1, true) != 0)
    return false;
  if (i2c.read(LSM6DSL_ADDR, &r, 1) != 0)
    return false;
  val = (uint8_t)r;
  return true;
}

bool read_int16(uint8_t reg_low, int16_t &val) {
  uint8_t lo, hi;
  if (!read_reg(reg_low, lo))
    return false;
  if (!read_reg(reg_low + 1, hi))
    return false;
  val = (int16_t)((hi << 8) | lo);
  return true;
}

bool init_sensor() {
  uint8_t who;
  if (!read_reg(WHO_AM_I, who) || who != 0x6A) {
    printf("Sensor not found!\r\n");
    return false;
  }

  write_reg(CTRL3_C, 0x44);        // Enable BDU + auto-increment
  write_reg(CTRL1_XL, 0x44);       // 104Hz, ±16g
  write_reg(CTRL2_G, 0x40);        // 104Hz, ±250 dps
  write_reg(INT1_CTRL, 0x03);      // Route data-ready to INT1 pin
  write_reg(DRDY_PULSE_CFG, 0x80); // Use 50us pulse on INT1

  ThisThread::sleep_for(100ms);

  // Clear any stale data
  uint8_t dummy;
  read_reg(STATUS_REG, dummy);
  int16_t temp;
  for (int i = 0; i < 6; i++) {
    read_int16(OUTX_L_XL + i * 2, temp);
  }

  // Attach ISR to rising edge of INT1 pin
  int1.rise(&data_ready_isr);

  return true;
}

void read_sensor_data() {
  int16_t acc[3], gyro[3];

  for (int i = 0; i < 3; i++) {
    read_int16(OUTX_L_XL + i * 2, acc[i]);
    read_int16(OUTX_L_G + i * 2, gyro[i]);
  }

  // ±16g: 0.488 mg/LSB → divide by 1000 for g
  float ax = acc[0] * 0.000488f;
  float ay = acc[1] * 0.000488f;
  float az = acc[2] * 0.000488f;

  // ±250 dps: 8.75 mdps/LSB → divide by 1000 for dps
  float gx = gyro[0] * 0.00875f;
  float gy = gyro[1] * 0.00875f;
  float gz = gyro[2] * 0.00875f;

  printf(">acc_x:%.3f\n>acc_y:%.3f\n>acc_z:%.3f\n"
         ">gyro_x:%.2f\n>gyro_y:%.2f\n>gyro_z:%.2f\n",
         ax, ay, az, gx, gy, gz);
}

int main() {
  static BufferedSerial pc(USBTX, USBRX, 115200);
  i2c.frequency(400000);

  if (!init_sensor()) {
    while (1) {
      ThisThread::sleep_for(1s);
    }
  }

  while (true) {
    if (data_ready) {
      data_ready = false; // Clear flag before reading
      read_sensor_data(); // Now safe to read I2C
    }
    ThisThread::sleep_for(1ms); // Prevent busy-waiting
  }
}