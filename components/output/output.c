#include "output.h"
#include "logic.h"
#include "app_config.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

static const char *TAG = "OUTPUTS";
static uint8_t relay_state = 0;     // updated by outputs_update()
static uint8_t pwm_percent = 0;     // updated by outputs_update()


/*
 * =========================================================
 *  MODE SELECTION
 *
 *  SIMULATION_MODE = 1 → No hardware used, only logs
 *  SIMULATION_MODE = 0 → Real PWM + GPIO outputs
 *
 *  Switch modes by changing the value below.
 * =========================================================
 */
#define SIMULATION_MODE 1



/* =========================================================
 *  SIMULATION MODE IMPLEMENTATION
 * ========================================================= */
#if SIMULATION_MODE

/**
 * @brief Set the PWM duty cycle (simulation mode).
 * @param duty The duty cycle as a fraction (0.0 to 1.0).
 */
static void set_pwm_duty(float duty)
{
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 1.0f) duty = 1.0f;

    ESP_LOGI(TAG, "[SIM] PWM duty = %.0f%%", duty * 100.0f);
}
/**
 * @brief Set the relay state (simulation mode).
 * @param closed True to close the relay, false to open.
 */

static void set_relay(bool closed)
{
    ESP_LOGI(TAG, "[SIM] Relay = %s", closed ? "ON" : "OFF");
}

/**
 * @brief Set the LED pattern based on the system state (simulation mode).
 * @param state The current system state.
 * 
 * In simulation mode, this function logs the LED pattern that would be displayed for each system state. The actual LED behavior is described in the log messages, such as "green slow blink" for PRECHARGE or "red solid" for FAULT. This allows us to verify that the correct LED patterns are being selected based on the system state without needing physical hardware.    
 * 
 */

static void set_led_pattern(system_state_t state)
{
    switch (state)
    {
        case STATE_IDLE:
            ESP_LOGI(TAG, "[SIM] LEDs: IDLE (all off)");
            break;

        case STATE_PRECHARGE:
            ESP_LOGI(TAG, "[SIM] LEDs: PRECHARGE (green slow blink)");
            break;

        case STATE_CHARGING:
            ESP_LOGI(TAG, "[SIM] LEDs: CHARGING (green solid)");
            break;

        case STATE_FLOAT:
            ESP_LOGI(TAG, "[SIM] LEDs: FLOAT (green dim)");
            break;

        case STATE_FAULT:
            ESP_LOGI(TAG, "[SIM] LEDs: FAULT (red solid)");
            break;
    }
}

void outputs_init(void)
{
    ESP_LOGI(TAG, "[SIM] Outputs initialised (simulation mode)");
}

/**
 * @brief Update the outputs based on the current system state (simulation mode).
 * 
 * In simulation mode, this function retrieves the current system state from the logic module and logs the corresponding actions for PWM duty cycle, relay state, and LED pattern. The actual hardware interactions are replaced with log messages that describe what would happen in a real implementation. This allows us to verify that the correct output states are being selected based on the system state without needing physical hardware. The function also updates the internal variables `relay_state` and `pwm_percent` to reflect the simulated output states for testing purposes.
 * 
 */

void outputs_update(void)
{
    system_state_t state = logic_get_state();

    switch (state)
    {
        case STATE_IDLE:
            set_pwm_duty(0.0f);
            set_relay(false);
            set_led_pattern(state);
            relay_state = 0;    
            pwm_percent = 0;
            break;

        case STATE_PRECHARGE:
            set_pwm_duty(0.2f);
            set_relay(true);
            set_led_pattern(state);
            relay_state = 1;
            pwm_percent = 20;
            break;

        case STATE_CHARGING:
            set_pwm_duty(0.7f);
            set_relay(true);
            set_led_pattern(state);
            relay_state = 1;
            pwm_percent = 70;
            break;

        case STATE_FLOAT:
            set_pwm_duty(0.3f);
            set_relay(true);
            set_led_pattern(state);
            relay_state = 1;
            pwm_percent = 30;   
            break;

        case STATE_FAULT:
            set_pwm_duty(0.0f);
            set_relay(false);
            set_led_pattern(state);
            relay_state = 0;
            pwm_percent = 0;
            break;
    }
}

#else   // ===================================================
        // REAL HARDWARE MODE IMPLEMENTATION
        // ===================================================



/*
 * ---------------------------------------------------------
 *  REAL HARDWARE PIN ASSIGNMENTS
 * ---------------------------------------------------------
 */
#define PWM_GPIO            GPIO_NUM_18
#define RELAY_GPIO          GPIO_NUM_25
#define LED_FAULT_GPIO      GPIO_NUM_26
#define LED_CHARGE_GPIO     GPIO_NUM_27

#define PWM_FREQUENCY_HZ    5000
#define PWM_RESOLUTION_BITS LEDC_TIMER_10_BIT
#define PWM_MAX_DUTY        ((1 << 10) - 1)



/*
 * ---------------------------------------------------------
 *  REAL HARDWARE INITIALISATION
 * ---------------------------------------------------------
 */
void outputs_init(void)
{
    ESP_LOGI(TAG, "Initialising REAL hardware outputs...");

    // PWM timer
    ledc_timer_config_t timer_cfg = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = PWM_RESOLUTION_BITS,
        .freq_hz          = PWM_FREQUENCY_HZ,
        .clk_cfg          = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer_cfg);

    // PWM channel
    ledc_channel_config_t channel_cfg = {
        .gpio_num       = PWM_GPIO,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0,
        .hpoint         = 0,
    };
    ledc_channel_config(&channel_cfg);

    // Relay GPIO
    gpio_config_t relay_cfg = {
        .pin_bit_mask = 1ULL << RELAY_GPIO,
        .mode         = GPIO_MODE_OUTPUT,
    };
    gpio_config(&relay_cfg);
    gpio_set_level(RELAY_GPIO, 0);

    // LED GPIOs
    gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << LED_FAULT_GPIO) | (1ULL << LED_CHARGE_GPIO),
        .mode         = GPIO_MODE_OUTPUT,
    };
    gpio_config(&led_cfg);

    gpio_set_level(LED_FAULT_GPIO, 0);
    gpio_set_level(LED_CHARGE_GPIO, 0);

    ESP_LOGI(TAG, "REAL outputs initialised");
}

/*
 * ---------------------------------------------------------
 *  REAL HARDWARE OUTPUT FUNCTIONS
 * ---------------------------------------------------------
 */
static void set_pwm_duty(float duty)
{
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 1.0f) duty = 1.0f;

    uint32_t raw = (uint32_t)(duty * PWM_MAX_DUTY);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, raw);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

/**
 * @brief Set the relay state (real hardware mode).
 * @param closed True to close the relay, false to open.
 */
static void set_relay(bool closed)
{
    gpio_set_level(RELAY_GPIO, closed ? 1 : 0);
}

static void set_led_pattern(system_state_t state)
{
    bool fault_led  = (state == STATE_FAULT);
    bool charge_led = (state == STATE_CHARGING || state == STATE_FLOAT);

    gpio_set_level(LED_FAULT_GPIO,  fault_led);
    gpio_set_level(LED_CHARGE_GPIO, charge_led);
}



/*
 * ---------------------------------------------------------
 *  MAIN OUTPUT UPDATE FUNCTION (REAL MODE)
 * ---------------------------------------------------------
 */
void outputs_update(void)
{
    system_state_t state = logic_get_state();

    switch (state)
    {
        case STATE_IDLE:
            set_pwm_duty(0.0f);
            set_relay(false);
            relay_state = 0;    
            pwm_percent = 0;
            set_led_pattern(state);
            break;

        case STATE_PRECHARGE:
            set_pwm_duty(0.2f);
            set_relay(true);
            set_led_pattern(state);
            relay_state = 1;
            pwm_percent = 20;
            break;

        case STATE_CHARGING:
            set_pwm_duty(0.7f);
            set_relay(true);
            set_led_pattern(state);
            relay_state = 1;
            pwm_percent = 70;
            break;

        case STATE_FLOAT:
            set_pwm_duty(0.3f);
            set_relay(true);
            set_led_pattern(state);
            relay_state = 1;
            pwm_percent = 30;   
            break;

        case STATE_FAULT:
            set_pwm_duty(0.0f);
            set_relay(false);
            set_led_pattern(state);
            relay_state = 0;    
            pwm_percent = 0;
            break;
    }
}


#endif  // SIMULATION_MODE


uint8_t outputs_get_relay_state(void)
{
    return relay_state;
}

uint8_t outputs_get_pwm_percent(void)
{
    return pwm_percent;
}
