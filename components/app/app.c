/**
 * @file app.c
 * @brief Main application logic for the PSU firmware.      
 * 
 * This file contains the implementation of the main control task that runs in a 
 * loop, reading sensor measurements, checking for safety faults, updating the logic
 *  state, and sending status updates over CAN. The control task is responsible 
 *  for orchestrating the various components of the PSU firmware to ensure safe 
 * and efficient operation. It reads measurements from the sensors, checks them 
 * against safety thresholds, updates the system status based on the logic module,
 *  and sends relevant information to the Control Panel for monitoring.
 * 
 *  The task runs at a fixed interval (e.g., every 1000 ms) to provide regular 
 *   updates on the PSU's status.  
 * 
 * @author Muhsin Atto  
 * @date 2026-04-27 
 * 
 * 
 */

#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "psu_can_tx.h"
#include "output.h"

void vControlTask(void *pvParameters)
{  

    // 1- Initialize the measurement system to set up the necessary hardware and software 
    // resources for reading sensor data. This may involve configuring ADC channels, 
    // initializing I2C or SPI interfaces for external sensors, and setting up any required 
    // timers or interrupts for periodic measurements. The measurement_init() function prepares
    // the system to read voltage, current, temperature, and tank levels accurately in the main control loop.
     
    measurement_init(); 

    // 2- Initialize the safety module to set up the thresholds and conditions for 
    //detecting faults in the PSU's operation. This may involve defining limits for
    // voltage, current, and temperature, as well as conditions for detecting issues 
    //with the water tanks (e.g., low fresh water level). The safety_init() function 
    // ensures that the system can monitor for unsafe conditions and take appropriate 
    // actions when faults are detected.
    safety_init();

    // 3- Initialize the logic module to set up the state machine and decision-making
    // processes for controlling the PSU. This may involve defining the various states
    // of the system (e.g., IDLE, CHARGING, ERROR) and the transitions between them 
    // based on sensor measurements and safety checks. The logic_init() function prepares
    // the system to make informed decisions about when to activate relays, adjust PWM outputs,
    // and manage battery charging based on the current conditions.
    logic_init();


    // 4- Initialize the output module to set up the GPIO pins and PWM channels for 
    // controlling the relays and other outputs of the PSU. This may involve configuring 
    //pin modes, setting initial states for relays, and preparing any necessary timers for PWM control. 
    // The outputs_init() function ensures that the system can effectively control 
    // the hardware components based on the decisions made by the logic module.    

    outputs_init();    
   // 5- Enter the main control loop, which runs indefinitely to continuously monitor the system's status and respond to changes. In each iteration of the loop, the following steps are performed:
    // - Read the latest sensor measurements (voltage, current, temperature, tank levels)   - Check for any safety faults based on the measurements and predefined thresholds.
    // - Update the logic state based on the current measurements and any detected faults.                              
    // - Update the outputs (relays, PWM) based on the current logic state to control the PSU's operation.
    // - Build the system status and tank levels frames to send over CAN for monitoring by the
    // Control Panel. The system status frame includes information about the relay state, PWM percentage, battery state, and charging mode, while the tank levels frame includes the percentages of fresh, grey, and black water in the tanks.
    // - Send the latest measurements, system status, and tank levels to the Control Panel over CAN using the psu_can_send_electrical(), psu_can_send_system_status(), and psu_can_send_tank_levels() functions. This allows the Control Panel to receive real-time updates on the PSU's status for display and monitoring purposes.

    TickType_t last_wake = xTaskGetTickCount();

    for (;;)
    {   

        // Read the latest sensor measurements for voltage, current, temperature, and tank levels.  
        measurements_t m = measurement_read();
        // Check for any safety faults based on the measurements and predefined thresholds. 

        safety_fault_t fault = safety_check(&m);
        //  Update the logic state based on the current measurements and any detected faults. The logic_step() function takes the current fault status and sensor measurements to determine the appropriate state of the system, such as whether to activate relays, adjust PWM outputs, or change charging modes. This function is responsible for making informed decisions to ensure safe and efficient operation of the PSU based on real-time data.    
        logic_step(fault, &m);
        //  Update the outputs (relays, PWM) based on the current logic state to control the PSU's operation. The outputs_update() function reads the current state from the logic module and sets the appropriate GPIO levels for relays and adjusts PWM outputs as needed to control the power supply unit according to the decisions made by the logic module. This ensures that the hardware components of the PSU respond correctly to changing conditions and maintain safe operation.    

        outputs_update();

        // Build system status frame
        system_status_t status = {
        .relay_state   = outputs_get_relay_state(), // Get the current state of the relays (e.g., which relays are active) to include in the system status frame. This information is crucial for monitoring the operational state of the PSU and diagnosing any issues related to relay control.
        .pwm_percent   = outputs_get_pwm_percent(), // Get the current PWM percentage to include in the system status frame. This value indicates how much power is being delivered to the load and is important for monitoring the performance of the PSU and ensuring it operates within safe limits.
        .battery_state = logic_get_battery_state(), // Get the current battery state (e.g., percentage charged, health status) from the logic module to include in the system status frame. This information provides insight into the condition of the battery being charged by the PSU and helps with monitoring and diagnostics.
        .charging_mode = logic_get_charging_mode() //   Get the current charging mode (e.g., IDLE, CHARGING, ERROR) from the logic module to include in the system status frame. This information provides insight into the current operational mode of the PSU and helps with monitoring and diagnostics.
       };

// Build tank levels frame
     tank_levels_t tanks = {
      .fresh_percent = measurement_get_fresh_level(), // Get the current percentage of fresh water in the tank to include in the tank levels frame. This information is important for monitoring the water supply status and ensuring that there is sufficient fresh water for the system's operation.
      .grey_percent  = measurement_get_grey_level(),  // Get the current percentage of grey water in the tank to include in the tank levels frame. This information is important for monitoring the status of the grey water tank and ensuring that it does not overflow or cause issues with the system's operation.
      .black_percent = measurement_get_black_level() // Get the current percentage of black water in the tank to include in the tank levels frame. This information is important for monitoring the status of the black water tank and ensuring that it does not overflow or cause issues with the system's operation.
   };
         
        // Send the latest measurements to the Control Panel over CAN   
        // This function packs the voltage, current, and temperature into   
        // a CAN frame with ID 0x120 and transmits it using the CAN driver. 
        //The measurements are scaled and converted to fit into the 8-byte data payload of the CAN frame. This allows the Control Panel to receive real-time updates of the PSU's electrical status for display and monitoring purposes.  

        
        psu_can_send_electrical(&m);  // Send electrical status (voltage, current, temperature)
        psu_can_send_system_status(&status); // Send system status (relay state, PWM, battery %, charging mode) 
        psu_can_send_tank_levels(&tanks); // Send tank levels (fresh/grey/black)
        //psu_can_send_fault_flags(fault); // Send fault flags (bitmask of current faults)
       
         // determinsitic loop delay to ensure the control task runs at a fixed interval (e.g., every 1000 ms).
         // The xTaskDelayUntil() function takes the last wake time and the desired interval (          
        //converted to ticks using pdMS_TO_TICKS()) to calculate the next wake time. This ensures that the control task executes at consistent intervals, regardless of how long the processing in each loop iteration takes. By using a deterministic delay, we can maintain a predictable update rate for reading sensors, checking safety, updating logic, and sending CAN messages, which is crucial for real-time monitoring and control of the PSU.       

        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(1000));
    }
}
