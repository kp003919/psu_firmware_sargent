/**
 * @file output.h
 * @brief Output control header defining functions for managing PWM, relays, and LEDs.      
 * This header declares the interface for the output control module, which is responsible for managing the hardware outputs of the system based on the current logic state. The module includes functions for initializing the output hardware and updating the outputs according to the system state determined by the logic module. The outputs may include PWM signals for controlling power levels, relay states for switching circuits, and LED patterns for visual status indication. The implementation of these functions will depend on the specific hardware being used and should be defined in the corresponding source file (output.c).
 * @author Muhsin Atto  
 * @date 2026-04-27
 * 
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include "logic.h"


/**
 * @brief Initialize the output hardware (PWM, relays, LEDs).   
 * This function configures the necessary hardware peripherals for controlling the outputs of the system, such as setting up PWM channels, configuring GPIOs for relay control, and initializing any LED drivers. It should be called once during system startup before any calls to outputs_update(). The specific initialization steps will depend on the hardware platform and may involve setting pin modes, configuring timers for PWM, and ensuring that all outputs are in a safe default state (e.g., relays open, LEDs off) before the control loop begins.   
 * @note The implementation of this function is hardware-specific and should be defined in output.c.    
 * @see outputs_update() for updating outputs based on system state.    
 * @warning Ensure that the output hardware is properly initialized before calling outputs_update() to avoid undefined behavior or hardware damage. 
 * 
 * @param None
 * @return None 
 * 
 */
void outputs_init(void);
/**
 * @brief Update the outputs based on the current system state. 
 * 
 *  
 */
void outputs_update(void);

/**
 *@brief Get the current relay state (for testing purposes).    
 *
 */
uint8_t outputs_get_relay_state(void);

/**
 * @brief Get the current PWM duty cycle percentage (for testing purposes).
 */
uint8_t outputs_get_pwm_percent(void);


#endif
