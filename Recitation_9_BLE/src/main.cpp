#include "mbed.h"
#include "ble/BLE.h" //  gives us Bluetooth Low Energy functionality
#include "ble/gatt/GattService.h" //  Services are how BLE organises data
#include "ble/gatt/GattCharacteristic.h" //Characteristics hold the actual data
#include "ble/Gap.h" //Handles how devices connect to each other
#include "ble/gap/AdvertisingDataBuilder.h" //helps create the "I'm here!" broadcast message
#include "events/EventQueue.h" //helps schedule tasks and timing
#include <string.h>


BufferedSerial serial_port(USBTX, USBRX, 115200);
FileHandle *mbed::mbed_override_console(int) {
    return &serial_port;  
}

// Make BLE and events functions available without prefixes
using namespace ble;
using namespace events;

// Get the single BLE instance that all code will use
BLE &ble_interface = BLE::Instance();  
// Create our event queue for scheduling tasks
EventQueue event_queue;


DigitalOut led(LED1);

// Create unique IDs for our service and characteristic
// These long numbers ensure our service doesn't conflict with others
const UUID TREMOR_SERVICE_UUID("A0E1B2C3-D4E5-F6A7-B8C9-D0E1F2A3B4C5");
const UUID TREMOR_TYPE_CHAR_UUID("A1E2B3C4-D5E6-F7A8-B9C0-D1E2F3A4B5C6");

// Define our TREMOR _ strings
const char* NONE = "NONE";
const char* TREMOR_STRING = "TREMOR";
const char* DYSKINESIA_STRING = "DYSKINESIA";

// Maximum string length for our TREMOR type (including null terminator)
#define MAX_TREMOR_STRING_LEN 11

// Buffer to hold our TREMOR type string
// Initialize with "TREMOR"
uint8_t TREMORValue[MAX_TREMOR_STRING_LEN];

// Create our BLE characteristic for the TREMOR type string
// ReadOnly: Other devices can only read this value, not write to it
// Notify: Connected devices can be notified when this value changes
ReadOnlyArrayGattCharacteristic<uint8_t, MAX_TREMOR_STRING_LEN> TREMORTypeCharacteristic(
    TREMOR_TYPE_CHAR_UUID,
    TREMORValue,
    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
);

// Put our characteristic in a table 
GattCharacteristic *charTable[] = { &TREMORTypeCharacteristic };

// Create our BLE service and add the characteristic to it
GattService TREMOR_Service(TREMOR_SERVICE_UUID, charTable, 1);

// Track whether we're in tremor (true) or dyskinesia (false) state
bool isTremor = true;

// Initialize with the tremor string
void init_TREMOR_value() {
    // Copy the tremor string to our buffer
    strcpy((char*)TREMORValue, NONE);
}

// This function sends a notification about the current TREMOR _ type
// It's called every second to toggle between tremor and dyskinesia
void send_TREMOR_notification() {
    // Set the string value based on our current state
    if (isTremor) {
        strcpy((char*)TREMORValue, TREMOR_STRING);
    } else {
        strcpy((char*)TREMORValue, DYSKINESIA_STRING);
    }
    
    // Send the notification to any connected devices
    ble_interface.gattServer().write(
        TREMORTypeCharacteristic.getValueHandle(),
        TREMORValue,
        strlen((char*)TREMORValue) + 1  // +1 for null terminator
    );
    
    // Print the current state for debugging
    printf("Sent notification: %s\n", (char*)TREMORValue);
    
    // Toggle the LED to show the state changed
    led = !led;
    
    // Switch between tremor and dyskinesia states
    isTremor = !isTremor;
}

// This function is called automatically when BLE finishes initializing
void on_ble_init_complete(BLE::InitializationCompleteCallbackContext *params) {
    // Check if there was an error during initialization
    if (params->error != BLE_ERROR_NONE) {
        printf("BLE initialization failed.\n");
        return;
    }
    
    // Initialize our TREMOR value to "TREMOR"
    init_TREMOR_value();
    
    // Add our TREMOR _ service to the BLE server
    ble_interface.gattServer().addService(TREMOR_Service);
    
    // Set up the advertising (how our device announces itself)
    // First create a buffer to hold the advertising data
    uint8_t adv_buffer[LEGACY_ADVERTISING_MAX_SIZE];
    AdvertisingDataBuilder adv_data(adv_buffer);
    
    // Set standard BLE flags
    adv_data.setFlags();
    
    // Set the device name that will appear when scanning
    adv_data.setName("TREMOR-Monitor");
    
    // Configure how often we advertise (160 * 0.625ms = 100ms)
    // 0.625ms is the standard BLE time unit
    ble_interface.gap().setAdvertisingParameters(
        LEGACY_ADVERTISING_HANDLE,
        AdvertisingParameters(advertising_type_t::CONNECTABLE_UNDIRECTED, adv_interval_t(160))
    );
    
    // Load our advertising data and start broadcasting
    ble_interface.gap().setAdvertisingPayload(LEGACY_ADVERTISING_HANDLE, adv_data.getAdvertisingData());
    ble_interface.gap().startAdvertising(LEGACY_ADVERTISING_HANDLE);
    
    printf("BLE advertising started as TREMOR-_-Monitor\n");
    
    // Schedule our notification function to run every 1000ms (1 second)
    event_queue.call_every(1000ms, send_TREMOR_notification);
}

// This function handles BLE events by putting them in our event queue
/*Acts as a callback that the BLE system invokes when it has events that need processing 
(like connection requests, disconnections, data transmissions)
Takes a parameter context which contains information about the pending BLE events
Rather than processing BLE events immediately, it puts them into the event queue using event_queue.call()
The callback(&ble_interface, &BLE::processEvents) creates a function pointer to the processEvents method 
of the BLE interface*/
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(callback(&ble_interface, &BLE::processEvents));
}



int main() {

    printf("Starting BLE TREMOR _ Monitor...\n");
    
    // Set up our event handler for BLE events
    ble_interface.onEventsToProcess(schedule_ble_events);
    
    // Initialize BLE with our callback function
    ble_interface.init(on_ble_init_complete);
    
    // Start the event queue running forever
    // This keeps our program running and processing events
    event_queue.dispatch_forever();
}