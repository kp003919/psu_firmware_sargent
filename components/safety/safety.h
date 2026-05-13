/**
 * @file safety.h
 * @brief Safety check header defining functions for validating measurements against safety thresholds.     
 * This header declares the interface for the safety module, which is responsible for checking sensor measurements against predefined safety thresholds and determining if any faults are present. The module includes an initialization function to set up any necessary state, a function to check the measurements and update the current fault status, and a helper function to determine if a given fault status indicates a fault condition. The implementation of these functions will be defined in the corresponding source file (safety.c) and will use thresholds defined in the config.h file.
 * @author Muhsin Atto      
 * @date 2026-04-27 
 * 
 */

#ifndef SAFETY_H
#define SAFETY_H

#include <stdbool.h>
#include "measurement.h"

// Define the possible safety fault conditions that can be detected based on the measurements.  
typedef enum
{
    SAFETY_OK = 0,
    SAFETY_FAULT_OVERVOLT,
    SAFETY_FAULT_UNDERVOLT,
    SAFETY_FAULT_OVERCURRENT,
    SAFETY_FAULT_OVERTEMP
} safety_fault_t;

/** @brief Initialize the safety module.
 * This function sets up any necessary state for the safety module, such as initializing thresholds or clearing fault flags.
 *  It should be called once during system startup before any calls to safety_check() to ensure that the module is in a known state. The specific initialization steps will depend on the implementation in safety.c, but may include setting the last_fault variable to SAFETY_OK or performing any necessary hardware checks. 
 * @note The implementation of this function is defined in safety.c and should be called before any safety checks are performed to ensure correct operation of the safety module.       
 * @see safety_check() for evaluating measurements against safety thresholds and updating fault status. 
 * 
 * 
 * @param None
 * @return None
 */
void safety_init(void);

/** @brief Check the current measurements against safety thresholds.
 * This function evaluates the provided measurements against 
 * the predefined safety thresholds and updates the current fault status
 *  accordingly.
 * - if the voltage exceeds MAX_VOLTAGE, the fault status is set to SAFETY_FAULT_OVERVOLT.
 * - if the voltage is below MIN_VOLTAGE, the fault status is set to SAFETY_FAULT_UNDERVOLT.
 * - if the current exceeds MAX_CURRENT, the fault status is set to SAFETY_FA ULT_OVERCURRENT.
 * - if the temperature exceeds MAX_TEMPERATURE, the fault status is set to SAFETY_FAULT_OVERTEMP. 
 * - if all measurements are within safe limits, the fault status is set to SAFETY_OK.
 * The function returns the updated fault status after evaluating the measurements. This allows the control logic to make decisions based on the current safety status of the system. The specific thresholds used for evaluation are defined in the config.h file, and the implementation of this function will be in safety.c.


 * @param m Pointer to the structure containing the current measurements.
 * @return The updated fault status based on the measurement evaluation.
 */
safety_fault_t safety_check(const measurements_t *m);

/** @brief Determine if a given fault status indicates a fault condition.
 * This helper function checks if the provided fault status is anything other than SAFETY_OK, which indicates that a fault condition has been detected. It returns true if a fault is present and false if the status is OK. This function can be used in the control logic to easily check for fault conditions without needing to compare against each specific fault type.
 * @param fault The safety_fault_t value to check.
 * @return   - true if the fault status indicates a fault condition,
 *           - false if it is SAFETY_OK.
 */  
bool safety_is_fault(safety_fault_t fault);

#endif
