#include <mbed.h>

DigitalOut led(LED2);

int main(){
    while(1){

        led = 1; // Turn the LED on
        thread_sleep_for(500); // Wait for 500 milliseconds
        //wait_us(500000); // Alternative wait function

        led = 0; // Turn the LED off
        thread_sleep_for(500); // Wait for 500 milliseconds
        //wait_us(500000); // Alternative wait function
    }
}