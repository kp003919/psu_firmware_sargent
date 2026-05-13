/**
 * @file main.c
 * @brief Main application entry point for the PSU firmware.    
 * 
 * This file initializes the CAN driver and starts the main control task that will handle the logic for monitoring and controlling the power supply unit (PSU). The control task will read sensor data, update the UI, and make decisions based on the measurements to ensure safe operation of the PSU. The CAN driver is responsible for communication with other components in the system, allowing for data exchange and coordination. The main function serves as the starting point for the application, setting up necessary components and launching the control task.  
 * @author Muhsin Atto      
 * @date 2026-04-27     
 * 
 * @copyright 2026 Sargent Embedded Team. 
 * All rights reserved.     
 */


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app.h"
#include "can_driver.h"


typedef struct {
    float voltage;
    float current;
    int pwm;
    int relay;
    int state;
    int fault;
} telemetry_t;

static telemetry_t g_telemetry;


void app_main(void)
{   
    // Initialize the CAN driver to enable communication with other components 
    // in the system. This function sets up the necessary hardware and software 
    // resources for CAN communication, allowing the PSU firmware to send and receive
    //  messages over the CAN bus. 
    
    // The initialization process may involve configuring 
    // GPIO pins, setting baud rates, and enabling interrupts for CAN events. 
    
    // Successful initialization is crucial for the proper functioning of 
    // the control task, as it relies on     // CAN communication to exchange 
    // data with sensors and other modules. 
    can_driver_init();

    // Create the main control task that will handle the logic for monitoring and 
    // controlling the PSU.
    // The task is created with a stack size of 4096 bytes and a priority of    
    // 5. The task will run the vControlTask function, which contains the main 
    // loop for reading sensor data, updating the UI, and making decisions based 
    // on the measurements.
    xTaskCreate(vControlTask, "control", 4096, NULL, 5, NULL);
}
