// #include "mbed.h"

// I2C i2c(PB_11, PB_10); // I2C2: SDA = PB11, SCL = PB10

// // ignore this sometimes mac needs this to properly use printf
// //  Create serial and bind it to printf
// BufferedSerial serial_port(USBTX, USBRX, 115200);
// FileHandle *mbed::mbed_override_console(int) { return &serial_port; }

// // LSM6DSL address (0x6A in datasheet, shifted left for 8-bit format)
// #define LSM6DSL_ADDR (0x6A << 1) // Equals 0xD4

// // Please Refer to 48 and 49th pages in LSM6DSL datasheet
// #define WHO_AM_I 0x0F  // ID register - should return 0x6A
// #define CTRL1_XL 0x10  // Accelerometer control register to configure range
// #define CTRL2_G 0x11   // Gyroscope control register to configure range
// #define OUTX_L_XL 0x28 // XL X-axis (low byte)
// #define OUTX_H_XL 0x29 // XL X-axis (high byte)
// #define OUTY_L_XL 0x2A // XL Y-axis (low byte)
// #define OUTY_H_XL 0x2B // XL Y-axis (high byte)
// #define OUTZ_L_XL 0x2C // XL Z-axis (low byte)
// #define OUTZ_H_XL 0x2D // XL Z-axis (high byte)
// #define OUTX_L_G 0x22  // Gyro X-axis (low byte)
// #define OUTX_H_G 0x23  // Gyro X-axis (high byte)
// #define OUTY_L_G 0x24  // Gyro Y-axis (low byte)
// #define OUTY_H_G 0x25  // Gyro Y-axis (high byte)
// #define OUTZ_L_G 0x26  // Gyro Z-axis (low byte)
// #define OUTZ_H_G 0x27  // Gyro Z-axis (high byte)

// // Write a value to a register
// void write_register(uint8_t reg, uint8_t value) {
//   char data[2] = {(char)reg, (char)value};
//   i2c.write(LSM6DSL_ADDR, data, 2);
// }

// // Read a value from a register
// uint8_t read_register(uint8_t reg) {
//   char data = reg;
//   i2c.write(LSM6DSL_ADDR, &data, 1, true); // No stop
//   i2c.read(LSM6DSL_ADDR, &data, 1);
//   return (uint8_t)data;
// }

// // Read a 16-bit value (combines low and high byte registers)
// int16_t read_16bit_value(uint8_t low_reg, uint8_t high_reg) {
//   // Read low byte
//   char low_byte = read_register(low_reg);

//   // Read high byte
//   char high_byte = read_register(high_reg);

//   // Combine the bytes (little-endian: low byte first)
//   return (high_byte << 8) | low_byte;
// }

// int main() {
//   // Setup I2C at 400kHz
//   i2c.frequency(400000);

//   // Check if sensor is connected
//   uint8_t id = read_register(WHO_AM_I);
//   printf("WHO_AM_I = 0x%02X (Expected: 0x6A)\r\n", id);

//   if (id != 0x6A) {
//     printf("Error: LSM6DSL sensor not found!\r\n");
//     while (1) { /* Stop here */
//     }
//   }

//    // Configure the accelerometer (104 Hz, ±2g range)

//    printf("Accelerometer configure accouridng to you requirement : ±2g range,
//    ±4g range, ±8g range, ±16g range\r\n");

//     write_register(CTRL1_XL, 0x40);
//     const float ACC_SENSITIVITY = 0.061f;  // mg/LSB for ±2g range

//   // Configure the gyroscope (104 Hz, ±250 dps range)

//     printf("Gyroscope configured: ±250 dps range, ±500 dps range, ±1000 dps
//     range, ±2000 dps range\r\n");

//     write_register(CTRL2_G, 0x40);
//     const float GYRO_SENSITIVITY = 8.75f; // mdps/LSB for ±250 dps range

//   // Main loop
//  while (1) {
//     // Read raw accelerometer values
//     int16_t acc_x_raw = read_16bit_value(OUTX_L_XL, OUTX_H_XL);
//     int16_t acc_y_raw = read_16bit_value(OUTY_L_XL, OUTY_H_XL);
//     int16_t acc_z_raw = read_16bit_value(OUTZ_L_XL, OUTZ_H_XL);

//     // Read raw gyroscope values
//     int16_t gyro_x_raw = read_16bit_value(OUTX_L_G, OUTX_H_G);
//     int16_t gyro_y_raw = read_16bit_value(OUTY_L_G, OUTY_H_G);
//     int16_t gyro_z_raw = read_16bit_value(OUTZ_L_G, OUTZ_H_G);

//     // Convert accelerometer values from raw to g
//     float acc_x_g = acc_x_raw * ACC_SENSITIVITY / 1000.0f;
//     float acc_y_g = acc_y_raw * ACC_SENSITIVITY / 1000.0f;
//     float acc_z_g = acc_z_raw * ACC_SENSITIVITY / 1000.0f;

//     // Convert gyroscope values from raw to dps
//     float gyro_x_dps = gyro_x_raw * GYRO_SENSITIVITY / 1000.0f;
//     float gyro_y_dps = gyro_y_raw * GYRO_SENSITIVITY / 1000.0f;
//     float gyro_z_dps = gyro_z_raw * GYRO_SENSITIVITY / 1000.0f;

//     // Print converted values using printf
//     printf("Accel [g]: X=%+6.3f, Y=%+6.3f, Z=%+6.3f | Gyro [dps]: X=%+7.2f, "
//            "Y=%+7.2f, Z=%+7.2f\r\n",
//            acc_x_g, acc_y_g, acc_z_g, gyro_x_dps, gyro_y_dps, gyro_z_dps);

//     // Output Teleplot format directly with printf
//     printf(">acc_x:%.3f\n>acc_y:%.3f\n>acc_z:%.3f\n"
//            ">gyro_x:%.2f\n>gyro_y:%.2f\n>gyro_z:%.2f\n",
//            acc_x_g, acc_y_g, acc_z_g, gyro_x_dps, gyro_y_dps, gyro_z_dps);

//     // Wait before next sample
//     ThisThread::sleep_for(200ms);
//   }
// }

// /**** Exercise 2 */

// #include "mbed.h"

// // Initialize I2C on pins PB_11 (SDA) and PB_10 (SCL)
// I2C i2c(PB_11, PB_10);

// // LSM6DSL I2C address (0x6A shifted left by 1 for Mbed's 8-bit addressing)
// #define LSM6DSL_ADDR        (0x6A << 1)
// // Register addresses
// #define WHO_AM_I            0x0F  // Device identification register
// #define CTRL1_XL            0x10  // Accelerometer control register
// #define CTRL2_G             0x11  // Gyroscope control register
// #define CTRL3_C             0x12  // Common control register
// #define DRDY_PULSE_CFG      0x0B  // Data-ready pulse configuration
// #define INT1_CTRL           0x0D  // INT1 pin routing control
// #define STATUS_REG          0x1E  // Status register (data ready flags)
// #define OUTX_L_G            0x22  // Gyroscope X-axis low byte start address
// #define OUTX_L_XL           0x28  // Accelerometer X-axis low byte start
// address

// // INT1 interrupt pin connected to PD_11
// #define LSM6DSL_INT1_PIN    PD_11

// // Configure INT1 as interrupt input with pull-down resistor
// InterruptIn int1(LSM6DSL_INT1_PIN, PullDown);
// // Flag set by interrupt when new data is ready
// volatile bool data_ready = false;

// // Interrupt service routine - sets flag when data is ready
// void data_ready_isr() {
//     data_ready = true;
// }

// // Write a single byte to a register
// bool write_reg(uint8_t reg, uint8_t val) {
//     // Create buffer with register address and value
//     char buf[2] = {(char)reg, (char)val};
//     // Write to I2C and return success status
//     return (i2c.write(LSM6DSL_ADDR, buf, 2) == 0);
// }

// // Read a single byte from a register
// bool read_reg(uint8_t reg, uint8_t &val) {
//     // Store register address to read from
//     char r = (char)reg;
//     // Write register address with repeated start condition
//     if (i2c.write(LSM6DSL_ADDR, &r, 1, true) != 0) return false;
//     // Read the register value
//     if (i2c.read(LSM6DSL_ADDR, &r, 1) != 0) return false;
//     // Store result in output parameter
//     val = (uint8_t)r;
//     return true;
// }

// // Read 16-bit signed integer from two consecutive registers
// bool read_int16(uint8_t reg_low, int16_t &val) {
//     uint8_t lo, hi;
//     // Read low byte
//     if (!read_reg(reg_low, lo)) return false;
//     // Read high byte from next register
//     if (!read_reg(reg_low + 1, hi)) return false;
//     // Combine bytes into 16-bit value (little-endian)
//     val = (int16_t)((hi << 8) | lo);
//     return true;
// }

// // Initialize the LSM6DSL sensor
// bool init_sensor() {
//     uint8_t who;
//     // Read WHO_AM_I register and verify it's 0x6A
//     if (!read_reg(WHO_AM_I, who) || who != 0x6A) {
//         printf("Sensor not found!\r\n");
//         return false;
//     }

//     // Configure CTRL3_C: Block data update + auto-increment address
//     write_reg(CTRL3_C, 0x44);
//     // Configure accelerometer: 104 Hz, ±16g range
//     write_reg(CTRL1_XL, 0x54);
//     // Configure gyroscope: 104 Hz, ±250 dps range
//     write_reg(CTRL2_G, 0x50);
//     // Route data-ready signal to INT1 pin
//     write_reg(INT1_CTRL, 0x03);
//     // Enable pulsed data-ready mode (50μs pulses)
//     write_reg(DRDY_PULSE_CFG, 0x80);

//     // Wait for sensor to stabilize
//     ThisThread::sleep_for(100ms);

//     // Clear status register
//     uint8_t dummy;
//     read_reg(STATUS_REG, dummy);
//     // Clear old data by reading all output registers
//     int16_t temp;
//     for (int i = 0; i < 6; i++) {
//         read_int16(OUTX_L_XL + i*2, temp);
//     }

//     // Attach interrupt handler for rising edge on INT1
//     int1.rise(&data_ready_isr);

//     return true;
// }

// // Read and print sensor data
// void read_sensor_data() {
//     // Arrays to store raw 16-bit values
//     int16_t acc[3], gyro[3];

//     // Read all 3 axes for accelerometer and gyroscope
//     for (int i = 0; i < 3; i++) {
//         read_int16(OUTX_L_XL + i*2, acc[i]);
//         read_int16(OUTX_L_G + i*2, gyro[i]);
//     }

//     // Convert accelerometer raw values to g (±16g range: 0.488 mg/LSB)
//     float ax = acc[0] * 0.000488f;
//     float ay = acc[1] * 0.000488f;
//     float az = acc[2] * 0.000488f;

//     // Convert gyroscope raw values to dps (±250 dps range: 8.75 mdps/LSB)
//     float gx = gyro[0] * 0.00875f;
//     float gy = gyro[1] * 0.00875f;
//     float gz = gyro[2] * 0.00875f;

//     // Print in Teleplot format (>name:value)
//     printf(">acc_x:%.3f\n>acc_y:%.3f\n>acc_z:%.3f\n>gyro_x:%.2f\n>gyro_y:%.2f\n>gyro_z:%.2f\n",
//     ax, ay, az, gx, gy, gz);
// }

// int main() {

//     static BufferedSerial pc(USBTX, USBRX, 115200);

//     // Set I2C clock speed to 400 kHz (fast mode)
//     i2c.frequency(400000);
//     // printf("\r\n=== LSM6DSL IMU Demo ===\r\n\r\n");

//     // Initialize sensor and halt if it fails
//     if (!init_sensor()) {
//         while(1) { ThisThread::sleep_for(1s); }
//     }

//     // printf("Sensor initialized. Streaming to Teleplot at 104
//     Hz...\r\n\r\n");

//     // Main loop: wait for interrupt and read data
//     while (true) {
//         // Check if new data is ready
//         if (data_ready) {
//             // Clear flag
//             data_ready = false;
//             // Read and print sensor data
//             read_sensor_data();
//         }
//         // Short sleep to prevent busy-waiting
//         ThisThread::sleep_for(1ms);
//     }
// }

/**** Exercise 3 */

#include "mbed.h"
#include <cstdio>

// Initialize I2C on pins PB_11 (SDA) and PB_10 (SCL)
I2C i2c(PB_11, PB_10);

// LSM6DSL I2C address (0x6A shifted left by 1 for Mbed's 8-bit addressing)
#define LSM6DSL_ADDR (0x6A << 1)
// Register addresses
#define WHO_AM_I 0x0F       // Device identification register
#define CTRL1_XL 0x10       // Accelerometer control register
#define CTRL2_G 0x11        // Gyroscope control register
#define CTRL3_C 0x12        // Common control register
#define DRDY_PULSE_CFG 0x0B // Data-ready pulse configuration
#define INT1_CTRL 0x0D      // INT1 pin routing control
#define STATUS_REG 0x1E     // Status register (data ready flags)
#define OUTX_L_G 0x22       // Gyroscope X-axis low byte start address
#define OUTX_L_XL 0x28      // Accelerometer X-axis low byte start address

// INT1 interrupt pin connected to PD_11
#define LSM6DSL_INT1_PIN PD_11

// Structure to hold one IMU sample
typedef struct {
  float acc[3];
  float gyro[3];
} ImuSample;

// Event queue for print task
EventQueue print_queue;

// Configure INT1 as interrupt input with pull-down resistor
InterruptIn int1(LSM6DSL_INT1_PIN, PullDown);
// Flag set by interrupt when new data is ready
volatile bool data_ready = false;

// Interrupt service routine - sets flag when data is ready
void data_ready_isr() { data_ready = true; }

// Write a single byte to a register
bool write_reg(uint8_t reg, uint8_t val) {
  // Create buffer with register address and value
  char buf[2] = {(char)reg, (char)val};
  // Write to I2C and return success status
  return (i2c.write(LSM6DSL_ADDR, buf, 2) == 0);
}

// Read a single byte from a register
bool read_reg(uint8_t reg, uint8_t &val) {
  // Store register address to read from
  char r = (char)reg;
  // Write register address with repeated start condition
  if (i2c.write(LSM6DSL_ADDR, &r, 1, true) != 0)
    return false;
  // Read the register value
  if (i2c.read(LSM6DSL_ADDR, &r, 1) != 0)
    return false;
  // Store result in output parameter
  val = (uint8_t)r;
  return true;
}

// Read 16-bit signed integer from two consecutive registers
bool read_int16(uint8_t reg_low, int16_t &val) {
  uint8_t lo, hi;
  // Read low byte
  if (!read_reg(reg_low, lo))
    return false;
  // Read high byte from next register
  if (!read_reg(reg_low + 1, hi))
    return false;
  // Combine bytes into 16-bit value (little-endian)
  val = (int16_t)((hi << 8) | lo);
  return true;
}

// Initialize the LSM6DSL sensor
bool init_sensor() {
  uint8_t who;
  // Read WHO_AM_I register and verify it's 0x6A
  if (!read_reg(WHO_AM_I, who) || who != 0x6A) {
    printf("Sensor not found!\r\n");
    return false;
  }

  // Configure CTRL3_C: Block data update + auto-increment address
  write_reg(CTRL3_C, 0x44);
  // Configure accelerometer: 104 Hz, ±16g range
  write_reg(CTRL1_XL, 0x54);
  // Configure gyroscope: 104 Hz, ±250 dps range
  write_reg(CTRL2_G, 0x50);
  // Route data-ready signal to INT1 pin
  write_reg(INT1_CTRL, 0x03);
  // Enable pulsed data-ready mode (50μs pulses)
  write_reg(DRDY_PULSE_CFG, 0x80);

  // Wait for sensor to stabilize
  ThisThread::sleep_for(100ms);

  // Clear status register
  uint8_t dummy;
  read_reg(STATUS_REG, dummy);
  // Clear old data by reading all output registers
  int16_t temp;
  for (int i = 0; i < 6; i++) {
    read_int16(OUTX_L_XL + i * 2, temp);
  }

  // Attach interrupt handler for rising edge on INT1
  int1.rise(&data_ready_isr);

  return true;
}

// Print function - called by event queue
void print_sample(ImuSample sample) {
  // Print in Teleplot format (>name:value)
  printf(">acc_x:%.3f\n>acc_y:%.3f\n>acc_z:%.3f\n>gyro_x:%.2f\n>gyro_y:%.2f\n>"
         "gyro_z:%.2f\n",
         sample.acc[0], sample.acc[1], sample.acc[2], sample.gyro[0],
         sample.gyro[1], sample.gyro[2]);
}

// Read sensor data and post to event queue
void read_sensor_data() {
  // Arrays to store raw 16-bit values
  int16_t acc[3], gyro[3];

  // Read all 3 axes for accelerometer and gyroscope
  for (int i = 0; i < 3; i++) {
    read_int16(OUTX_L_XL + i * 2, acc[i]);
    read_int16(OUTX_L_G + i * 2, gyro[i]);
  }

  // Create sample struct
  ImuSample sample;
  // Convert accelerometer raw values to g (±16g range: 0.488 mg/LSB)
  sample.acc[0] = acc[0] * 0.000488f;
  sample.acc[1] = acc[1] * 0.000488f;
  sample.acc[2] = acc[2] * 0.000488f;

  // Convert gyroscope raw values to dps (±250 dps range: 8.75 mdps/LSB)
  sample.gyro[0] = gyro[0] * 0.00875f;
  sample.gyro[1] = gyro[1] * 0.00875f;
  sample.gyro[2] = gyro[2] * 0.00875f;

  // Post print event to queue with struct
  print_queue.call(print_sample, sample);
}

// Acquisition task - reads sensor when data is ready
void acquisition_task() {
  while (true) {
    // Check if new data is ready
    if (data_ready) {
      // Clear flag
      data_ready = false;
      // Read and queue sensor data
      read_sensor_data();
    }
    // Short sleep to prevent busy-waiting
    ThisThread::sleep_for(1ms);
  }
}

// Print task - dispatches event queue
void print_task() { print_queue.dispatch_forever(); }

int main() {
  // Configure serial port
  static BufferedSerial pc(USBTX, USBRX, 115200);

  // Set I2C clock speed to 400 kHz (fast mode)
  i2c.frequency(400000);

  // Initialize sensor and halt if it fails
  if (!init_sensor()) {
    while (1) {
      ThisThread::sleep_for(1s);
    }
  }

  // Create acquisition thread
  Thread acq_thread;
  acq_thread.start(acquisition_task);

  // Create print thread
  Thread print_thread;
  print_thread.start(print_task);

  // Main thread does nothing, other threads handle everything
  while (true) {
    ThisThread::sleep_for(1s);
  }
}
