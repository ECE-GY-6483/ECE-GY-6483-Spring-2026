# Recitation 1: Introduction to Mbed and Basic GPIO

This recitation introduces the basic structure of an Mbed OS application and demonstrates 
simple GPIO control using an LED blink example.


---

## Development Setup

Please complete the development environment setup before running the examples in this recitation.

Refer to the setup guide provided in:
- `Recitation 1 - Environmental Setup.pdf`

---

## Demo Codes

### 1. Bare-Bones Mbed Application

This example shows the minimal structure of an Mbed program.

```cpp
#include "mbed.h"

// setup
int main() {
    while (true) {
        // loop that runs forever
    }
}

```

### 2. LED Blink with Serial Output


```cpp
#include <mbed.h>

DigitalOut led(LED1);

int main() {

    float value = 3.14159f;

    while (true) {
        // Toggle LED
        led = !led;

        printf("LED state: %d\n", led.read());
        printf("The value of pi is approximately: %.2f\n", value);

        // 1 second delay
        thread_sleep_for(1000);
    }
}
```

