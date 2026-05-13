#pragma once

#include "measurement.h"
#include "can_protocol.h"



/* ---------------------------------------------------------
 *  psu_can_tx.h
 *
 *  This module implements the CAN transmission functions for
 *  the PSU firmware. It provides functions to send various
 *  types of data (electrical status, system status, tank levels,
 *  fault flags) from the PSU to the Control Panel over CAN.
 *
 *  Each function packs the relevant data into a CAN frame with
 *  a specific identifier (defined in can_protocol.h) and uses
 *  the CAN driver to transmit it.
 *
 *  The data is typically scaled and converted to fit into the
 *  8-byte data payload of the CAN frame, allowing real-time
 *  updates of the PSU's status for display and monitoring on
 *  the Control Panel.
 * ---------------------------------------------------------
 */

 // Data structures for system status and tank levels to be sent over CAN
 // These structures define the fields that will be packed into the CAN frames for system status and tank levels. The system_status_t structure includes fields for relay state, PWM percentage, battery state, and charging mode, which represent the operational status of the PSU. The tank_levels_t structure includes percentage values for fresh water, grey water, and black water tanks, which represent the current levels of these tanks. These structures will be used as parameters in the corresponding CAN transmission functions to send this information to the Control Panel.  

typedef struct {
    uint8_t relay_state;
    uint8_t pwm_percent;
    uint8_t battery_state;
    uint8_t charging_mode;
} system_status_t;

// Tank levels structure to be sent over CAN frame 0x122    

typedef struct {
    uint8_t fresh_percent;
    uint8_t grey_percent;
    uint8_t black_percent;
} tank_levels_t;


/**
 * @brief Send electrical status (voltage, current, temperature)
 *        from the PSU to the Control Panel over CAN.
 *
 * This packs the measurements into CAN frame 0x120.
 */
void psu_can_send_electrical(const measurements_t *m);

/**
 * @brief Send system status (relay state, PWM, battery %, charging mode)
 *        from the PSU to the Control Panel over CAN.   
 * This packs the system status into CAN frame 0x121. The system_status_t structure should contain the relevant fields for relay state, PWM percentage, battery state, and charging mode, which will be packed into the data payload of the CAN frame for transmission to the Control Panel. This allows the Control Panel to receive real-time updates on the PSU's operational status for display and monitoring purposes.    
 *  * @param s Pointer to the system_status_t structure containing the current system status values.    
 * @return None
 */
void psu_can_send_system_status(const system_status_t *s);
/**
 * @brief Send tank levels (fresh/grey/black) from the PSU to the Control Panel over CAN.
 *
 * This packs the tank levels into CAN frame 0x122. The tank_levels_t structure should contain the percentage values for fresh water, grey water, and black water tanks, which will be packed into the data payload of the CAN frame for transmission to the Control Panel. This allows the Control Panel to receive real-time updates on the tank levels for display and monitoring purposes.    
 * @param t Pointer to the tank_levels_t structure containing the current tank level percentages.    
 * @return None
 */
void psu_can_send_tank_levels(const tank_levels_t *t);
/**
 * @brief Send fault flags from the PSU to the Control Panel over CAN.
 *
 * This packs the fault flags into CAN frame 0x123. The fault_mask parameter should be a bitmask representing the current fault conditions, which will be packed into the data payload of the CAN frame for transmission to the Control Panel. This allows the Control Panel to receive real-time updates on any faults detected by the PSU for display and monitoring purposes.
 * @param fault_mask Bitmask representing the current fault conditions.
 * @return None
 */
void psu_can_send_fault_flags(uint8_t fault_mask);
