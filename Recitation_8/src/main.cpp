#include "mbed.h"
#include "arm_math.h"

// Initialize I2C on pins PB_11 (SDA) and PB_10 (SCL)
I2C i2c(PB_11, PB_10);

// Create serial and bind it to printf (Required for Teleplot)
BufferedSerial serial_port(USBTX, USBRX, 115200);
FileHandle *mbed::mbed_override_console(int) {
    return &serial_port;
}


// LSM6DSL I2C address (0x6A shifted left by 1 for Mbed's 8-bit addressing)
#define LSM6DSL_ADDR        (0x6A << 1)

// Register addresses
#define WHO_AM_I            0x0F  // Device identification register
#define CTRL1_XL            0x10  // Accelerometer control register
#define CTRL2_G             0x11  // Gyroscope control register
#define CTRL3_C             0x12  // Common control register
#define OUTX_L_XL           0x28  // Accelerometer X-axis low byte
#define OUTX_H_XL           0x29  // Accelerometer X-axis high byte
#define OUTY_L_XL           0x2A  // Accelerometer Y-axis low byte
#define OUTY_H_XL           0x2B  // Accelerometer Y-axis high byte
#define OUTZ_L_XL           0x2C  // Accelerometer Z-axis low byte
#define OUTZ_H_XL           0x2D  // Accelerometer Z-axis high byte

// DSP Parameters
#define MA_WINDOW       10       // Moving Average Window Size
#define FFT_SIZE        256      // FFT Size (Must be power of 2: 128, 256, 512)
#define SAMPLE_RATE     104.0f   // Sensor ODR in Hz

// Derived Resolution: 104 / 256 = ~0.4 Hz per bin
const float FREQ_BIN_SIZE = SAMPLE_RATE / FFT_SIZE;

// Moving Average Buffers
float ma_buffer[MA_WINDOW] = {0};
int ma_idx = 0;
float ma_sum = 0.0f;

// FFT Buffers
float fft_input[FFT_SIZE];
float fft_output[FFT_SIZE];
float fft_mag[FFT_SIZE / 2];
int fft_idx = 0;

// CMSIS-DSP Instance
arm_rfft_fast_instance_f32 S;

// Global variable to hold the calculated frequency for printing
volatile float current_dominant_freq = 0.0f;


// Write a single byte to a register
void write_reg(uint8_t reg, uint8_t val) {
    char data[2] = {(char)reg, (char)val};
    i2c.write(LSM6DSL_ADDR, data, 2);
}

// Read a single byte from a register
bool read_reg(uint8_t reg, uint8_t &val) {
    char r = (char)reg;
    // Write register address with repeated start condition
    if (i2c.write(LSM6DSL_ADDR, &r, 1, true) != 0) return false;
    // Read the register value
    if (i2c.read(LSM6DSL_ADDR, &r, 1) != 0) return false;
    val = (uint8_t)r;
    return true;
}

// Read 16-bit signed integer from two consecutive registers
int16_t read_int16(uint8_t reg_low) {
    uint8_t lo, hi;
    // Read low byte
    read_reg(reg_low, lo);
    // Read high byte (next register)
    read_reg(reg_low + 1, hi);
    // Combine bytes into 16-bit value (little-endian)
    return (int16_t)((hi << 8) | lo);
}

// Initialize the LSM6DSL sensor
bool init_sensor() {
    uint8_t who;
    
    // Read WHO_AM_I register and verify it's 0x6A
    if (!read_reg(WHO_AM_I, who) || who != 0x6A) {
        printf("Sensor not found! WHO_AM_I = 0x%02X\r\n", who);
        return false;
    }

    // Configure Block Data Update (BDU)
    write_reg(CTRL3_C, 0x44); 

    // Configure Accelerometer: 104 Hz ODR, +/- 8g range, 400Hz Analog Filter
    // 0x4C = 0100 1100 (ODR=104Hz, FS=8g, BW=400Hz)
    write_reg(CTRL1_XL, 0x4C); 
    
    // Configure Gyroscope (Optional, not used in this specific demo)
    write_reg(CTRL2_G, 0x00); // Power down gyro to save power

    // Wait for sensor to stabilize
    ThisThread::sleep_for(100ms);
    return true;
}

// MAIN PROGRAM

int main() {
    // Setup I2C at 400kHz (Fast Mode)
    i2c.frequency(400000);

    printf("\r\n=== DSP Demo: Moving Average & FFT ===\r\n");
    printf("Open Teleplot to view graphs.\r\n\r\n");

    // 1. Initialize the FFT Library
    // This generates the lookup tables needed for fast math
    arm_rfft_fast_init_f32(&S, FFT_SIZE);

    // 2. Initialize Sensor
    if (!init_sensor()) {
        while(1) { 
            printf("Sensor Init Failed.\r\n");
            ThisThread::sleep_for(1s); 
        }
    }

    // Main Loop
    while (true) {
        
        // STEP A: DATA ACQUISITION         
        // Read Raw Accelerometer Z (Vertical Axis)
        int16_t raw_val = read_int16(OUTZ_L_XL);
        
        // Convert to Gravity (g)
        // Sensitivity for +/- 8g is 0.244 mg/LSB
        float acc_z = raw_val * 0.244f / 1000.0f;

        //STEP B: PRE-PROCESSING 
        
        // Remove Gravity (DC Offset) for the FFT
        // FFT works best on signals centered around 0
        float acc_z_centered = acc_z - 1.0f;


        //STEP C: DSP 1 - MOVING AVERAGE (Time Domain) 
        
        // Update Circular Buffer
        ma_sum -= ma_buffer[ma_idx];       // Subtract oldest
        ma_buffer[ma_idx] = acc_z;         // Add newest (using raw+gravity for visual context)
        ma_sum += ma_buffer[ma_idx];       // Add to sum
        ma_idx = (ma_idx + 1) % MA_WINDOW; // Move index
        
        // Calculate Average
        float filtered_acc_z = ma_sum / MA_WINDOW;


        //STEP D: DSP 2 - FFT (Frequency Domain)
        
        // Add sample to FFT input buffer
        fft_input[fft_idx] = acc_z_centered;
        fft_idx++;

        // Check if buffer is full
        if (fft_idx >= FFT_SIZE) {
            // 1. Compute FFT (Real -> Complex)
            arm_rfft_fast_f32(&S, fft_input, fft_output, 0);
            
            // 2. Compute Magnitude (Complex -> Real)
            arm_cmplx_mag_f32(fft_output, fft_mag, FFT_SIZE / 2);
            
            // 3. Find Dominant Frequency (Peak)
            float max_val = 0.0f;
            int max_bin = 0;
            
            // Start at i=1 to skip DC bias (0 Hz)
            for (int i = 1; i < FFT_SIZE / 2; i++) {
                if (fft_mag[i] > max_val) {
                    max_val = fft_mag[i];
                    max_bin = i;
                }
            }
            
            // 4. Convert Bin Index to Hz
            // Only update if signal is strong enough (Noise Threshold > 1.0)
            if (max_val > 1.0f) {
                current_dominant_freq = max_bin * FREQ_BIN_SIZE;
            } else {
                current_dominant_freq = 0.0f;
            }
            
            // Reset index to start filling buffer again
            fft_idx = 0;
        }


        //VISUALIZATION 
        
        // Print in Teleplot format
        // Note: Freq_Hz updates slower (every 2.5s) than Accel (every 10ms)
        printf(">Raw_Acc:%.2f\n", acc_z);
        printf(">Filtered_Acc:%.2f\n", filtered_acc_z);
        printf(">Freq_Hz:%.2f\n", current_dominant_freq);

        // Sampling Delay
        // 104 Hz = approx 9.6ms period
        ThisThread::sleep_for(9ms);
    }
}