#ifndef LOGIC_H
#define LOGIC_H

#include "safety.h"
#include "measurement.h"

// Define the possible system states for the control logic. These states represent the different modes of operation for the system, such as idle, precharge, charging, float, and fault conditions. The logic module will transition between these states based on the safety status and measurements, and the outputs will be updated accordingly to reflect the current state of the system.  
typedef enum
{
    STATE_IDLE = 0,
    STATE_PRECHARGE,
    STATE_CHARGING,
    STATE_FLOAT,
    STATE_FAULT
} system_state_t;

/** @brief Initialize the control logic module.
 * This function sets up any necessary state for the control logic module, such as initializing the system state or clearing any flags.
 * @param None
 * @return None
 */
void logic_init(void);

/** @brief Perform a step in the control logic.
 * This function evaluates the current safety status and measurements, and updates the system state accordingly.
 * @param fault The current safety fault status.
 * @param m Pointer to the structure containing the current measurements.
 * @return None
 */
void logic_step(safety_fault_t fault, const measurements_t *m);

/** @brief Get the current system state.
 * This function returns the current state of the system based on the control logic evaluation.
 * @param None
 * @return The current system state.
 */
system_state_t logic_get_state(void);

/** @brief Get the current battery state.
 * This function returns the current state of the battery based on the control logic evaluation.
 * @param None
 * @return The current battery state.
 */
uint8_t logic_get_battery_state(void);

/** @brief Get the current charging mode.
 * This function returns the current charging mode based on the control logic evaluation.
 * @param None
 * @return The current charging mode.
 */
uint8_t logic_get_charging_mode(void);






#endif
