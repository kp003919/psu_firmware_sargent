
/**
 
 * @brief Measurement header defining the interface for reading sensor data.    
 *  
 * This header declares the structure for holding sensor measurements (voltage, current, temperature) and the functions for initializing the measurement system and reading the current measurements. The implementation of these functions will depend on the specific hardware being used and should be defined in the corresponding source file (measurement.c). The measurement module is responsible for interfacing with the ADC or other sensors to obtain real-time data that will be used by the safety and logic modules to make decisions about the system's operation.
 * @author Muhsin Atto      
 * @date 2026-04-27 
 */
#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <stdint.h>

// Define a structure to hold the sensor measurements for voltage,
// current, and temperature. This structure will be populated by the measurement_read() function with the latest readings from the sensors. The fields are defined as floats to allow for decimal values, which are common in sensor readings. The specific units for each field (volts, amps, degrees Celsius) should be consistent with the hardware being used and the expected ranges of the measurements.  
typedef struct
{
    float voltage;    // V
    float current;    // A
    float temperature; // °C
} measurements_t;

/** @brief Initialize the measurement system.
 * This function sets up any necessary state for the measurement system, such as initializing the ADC or other sensors.
 * @param None
 * @return None
 */
void measurement_init(void);

/** @brief Read the current sensor measurements.
 * This function reads the latest values from the sensors and populates the measurements_t structure with the data.
 * @param None
 * @return The current sensor measurements.
 */
measurements_t measurement_read(void);

/** @brief Get the current fresh level.
 * This function returns the current fresh level based on the measurement evaluation.
 * @param None
 * @return The current fresh level.
 */
uint8_t measurement_get_fresh_level(void);

/** @brief Get the current grey level.
 * This function returns the current grey level based on the measurement evaluation.
 * @param None
 * @return The current grey level.
 */

uint8_t measurement_get_grey_level(void);
/** @brief Get the current black level.
 * This function returns the current black level based on the measurement evaluation.
 * @param None
 * @return The current black level.
 */
uint8_t measurement_get_black_level(void);


#endif
