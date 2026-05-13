#include "measurement.h"
#include "app_config.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_log.h"

static const char *TAG = "MEASUREMENT";
// Define SIMULATION_MODE to 1 to use simulated measurements instead of real ADC reads. 
#define SIMULATION_MODE 1




// Handles for ADC unit and calibration
static adc_oneshot_unit_handle_t adc1_handle;
static adc_cali_handle_t adc1_cali_handle;

static uint8_t fresh_level = 0;   // updated by measurement_read()
static uint8_t grey_level  = 0;
static uint8_t black_level = 0;



/*
 * ---------------------------------------------------------
 *  measurement_init()
 *
 *  This function configures the ESP32 ADC hardware.
 *  It performs three major tasks:
 *
 *   1. Create an ADC "unit" (ADC1)
 *   2. Configure each ADC channel (voltage, current, temp)
 *   3. Enable ADC calibration (line fitting)
 *
 *  After this function runs, the ADC is ready for real reads.
 * ---------------------------------------------------------
 */
void measurement_init(void)
{
    ESP_LOGI(TAG, "Initialising ADC...");

    // -------------------------------
    // 1. Create ADC1 unit
    // -------------------------------
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,   // Always use ADC1 (ADC2 conflicts with WiFi)
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc1_handle));

    // -------------------------------
    // 2. Configure ADC channels
    // -------------------------------
    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = ADC_BITWIDTH_12,  // 12-bit resolution (0–4095)
        .atten = ADC_ATTEN_DB_11,     // Allows reading up to ~3.3V
    };

    // Configure each channel defined in config.h
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_VOLTAGE, &chan_cfg));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_CURRENT, &chan_cfg));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_TEMP,    &chan_cfg));

    // -------------------------------
    // 3. Enable ADC calibration
    //
    // Calibration improves accuracy by compensating
    // for internal ADC non‑linearity.
    // -------------------------------
    adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_12,
    };

    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_cfg, &adc1_cali_handle));

    ESP_LOGI(TAG, "ADC initialised successfully");
}

/*
 * ---------------------------------------------------------
 *  read_voltage_from_channel()
 *
 *  Reads a single ADC channel and returns the voltage in V.
 *
 *  Steps:
 *    1. Trigger ADC conversion
 *    2. Read raw ADC value (0–4095)
 *    3. Convert raw → millivolts using calibration
 *    4. Convert millivolts → volts (float)
 * ---------------------------------------------------------
 */
static float read_voltage_from_channel(adc_channel_t ch)
{
    int raw = 0;

    // Trigger ADC conversion and read raw value
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ch, &raw));

    int mv = 0;

    // Convert raw ADC value to millivolts using calibration
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, raw, &mv));

    // Convert mV → V
    return (float)mv / 1000.0f;
}

/*
 * ---------------------------------------------------------
 *  measurement_read()
 *
 *  Reads all measurement channels and converts them into
 *  real‑world engineering units:
 *
 *    - Voltage (V)
 *    - Current (A)
 *    - Temperature (°C)
 *
 *  The conversion formulas depend on your hardware:
 *    - Voltage divider gain
 *    - Current sensor offset/gain
 *    - Temperature sensor offset/gain
 *
 *  These are defined in config.h.
 * ---------------------------------------------------------
 */
measurements_t measurement_read(void)
{
    measurements_t m;

#if SIMULATION_MODE
    m.voltage = 12.5f;
    m.current = 2.0f;
    m.temperature = 25.0f;
    return m;
#endif

    // REAL ADC MODE BELOW
    float v_adc = read_voltage_from_channel(ADC_CHANNEL_VOLTAGE);
    float i_adc = read_voltage_from_channel(ADC_CHANNEL_CURRENT);
    float t_adc = read_voltage_from_channel(ADC_CHANNEL_TEMP);

    m.voltage     = v_adc * VOLTAGE_DIVIDER_GAIN;
    m.current     = (i_adc - CURRENT_SENSOR_OFFSET) * CURRENT_SENSOR_GAIN;
    m.temperature = (t_adc - TEMP_SENSOR_OFFSET) * TEMP_SENSOR_GAIN;
    fresh_level   = (uint8_t)(m.voltage * 10);  // Example conversion, replace with actual logic
    grey_level    = (uint8_t)(m.current * 10);
    black_level   = (uint8_t)(m.temperature * 10);  

    return m;
}

uint8_t measurement_get_fresh_level(void)
{
    return fresh_level;
}

uint8_t measurement_get_grey_level(void)
{
    return grey_level;
}

uint8_t measurement_get_black_level(void)
{
    return black_level;
}
