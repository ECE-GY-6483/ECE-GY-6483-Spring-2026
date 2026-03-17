#include "mbed.h"

Timer t;
DigitalOut led(LED1);

int main() {
    t.start();
    while (true) {
        if (t.elapsed_time() >= 500ms) {
            led = !led;
            t.reset();
        }
        // Other work can happen here between checks
    }
}