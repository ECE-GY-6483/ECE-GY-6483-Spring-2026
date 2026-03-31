#include <mbed.h>

DigitalOut led(LED2);

int main(){
    while(1){
        printf("Toggle led\n");
        led = 1; // Turn the LED on
        thread_sleep_for(2000); // Wait for 500 milliseconds
        //wait_us(500000); // Alternative wait function
        printf("led turned on\n");
        led = 0; // Turn the LED off
        thread_sleep_for(2000); // Wait for 500 milliseconds
        //wait_us(500000); // Alternative wait function
        printf("led turned off\n");
    }
}