#ifndef CONFIG_H
#define CONFIG_H
/**
 * @file config.h
 * @brief Global configuration header for the control system application.
 *
 * This header defines all the global constants and configuration parameters used across the measurement, safety, logic, and output modules. It includes settings for ADC channel assignments, calibration factors for converting raw sensor readings to real-world units, safety limits for voltage, current, and temperature, thresholds for control logic state transitions, and pin assignments for outputs in real hardware mode. The SIMULATION_MODE switch allows the application to run in a simulated environment without requiring actual hardware, making it easier to test and develop the control logic before deploying on real hardware.
 * @author Muhsin Atto
 * @date 2026-04-27
 */

/*
 * =========================================================
 *  GLOBAL SIMULATION MODE SWITCH
 *
 *  SIMULATION_MODE = 1 → No hardware required
 *      - measurement.c returns simulated values
 *      - outputs.c prints simulated actions
 *
 *  SIMULATION_MODE = 0 → Real hardware mode
 *      - ADC reads real voltages
 *      - PWM, relay, LEDs use real GPIO
 *
 *  Change this value to switch modes.
 * =========================================================
 */
#define SIMULATION_MODE 1



/*
 * =========================================================
 *  ADC CHANNEL ASSIGNMENTS (REAL MODE ONLY)
 *
 *  These are ignored when SIMULATION_MODE = 1.
 *  They must match your ESP32 wiring when using real sensors.
 *
 *  ADC1 is used because ADC2 conflicts with Wi‑Fi.
 * =========================================================
 */


#define ADC_CHANNEL_VOLTAGE   ADC_CHANNEL_6   // GPIO34
#define ADC_CHANNEL_CURRENT   ADC_CHANNEL_7   // GPIO35
#define ADC_CHANNEL_TEMP      ADC_CHANNEL_4   // GPIO32



/*
 * =========================================================
 *  VOLTAGE MEASUREMENT CALIBRATION
 *
 *  Used only when SIMULATION_MODE = 0.
 *
 *  VOLTAGE_DIVIDER_GAIN converts ADC voltage → real voltage.
 *  Example divider:
 *      R1 = 47k
 *      R2 = 10k
 *      Gain = (R1 + R2) / R2 = 5.7
 * =========================================================
 */
#define VOLTAGE_DIVIDER_GAIN  5.7f



/*
 * =========================================================
 *  CURRENT MEASUREMENT CALIBRATION
 *
 *  Used only when SIMULATION_MODE = 0.
 *
 *  CURRENT_SENSOR_OFFSET = voltage at 0A
 *  CURRENT_SENSOR_GAIN   = amps per volt
 *
 *  Example for ACS712:
 *      Offset = 1.65V
 *      Gain   = 20 A/V
 * =========================================================
 */
#define CURRENT_SENSOR_OFFSET 1.65f
#define CURRENT_SENSOR_GAIN   20.0f



/*
 * =========================================================
 *  TEMPERATURE MEASUREMENT CALIBRATION
 *
 *  Used only when SIMULATION_MODE = 0.
 *
 *  Example for TMP36:
 *      0.5V = 0°C
 *      +10mV per °C
 * =========================================================
 */
#define TEMP_SENSOR_OFFSET    0.5f
#define TEMP_SENSOR_GAIN      100.0f



/*
 * =========================================================
 *  SAFETY LIMITS (USED IN BOTH MODES)
 *
 *  These thresholds apply regardless of simulation or real
 *  hardware. They define when the safety layer triggers a
 *  fault condition.
 * =========================================================
 */
#define MAX_VOLTAGE           14.8f
#define MIN_VOLTAGE           10.0f
#define MAX_CURRENT           20.0f
#define MAX_TEMPERATURE       70.0f



/*
 * =========================================================
 *  LOGIC LAYER THRESHOLDS (USED IN BOTH MODES)
 *
 *  These define when the state machine transitions between:
 *      - IDLE
 *      - PRECHARGE
 *      - CHARGING
 *      - FLOAT
 * =========================================================
 */
#define PRECHARGE_VOLTAGE     12.0f
#define FLOAT_VOLTAGE         14.4f



/*
 * =========================================================
 *  OUTPUTS LAYER PIN ASSIGNMENTS (REAL MODE ONLY)
 *
 *  These are ignored when SIMULATION_MODE = 1.
 *  They must match your wiring when using real hardware.
 * =========================================================
 */
#define PWM_GPIO_PIN          18
#define RELAY_GPIO_PIN        25
#define LED_FAULT_GPIO_PIN    26
#define LED_CHARGE_GPIO_PIN   27



/*
 * =========================================================
 *  PWM CONFIGURATION (REAL MODE ONLY)
 *
 *  Used by LEDC driver when SIMULATION_MODE = 0.
 * =========================================================
 */
#define PWM_FREQUENCY_HZ      5000
#define PWM_RESOLUTION_BITS   10

#endif // CONFIG_H
