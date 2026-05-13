/**
 * @file logic.c
 * @brief Control logic state machine for PSU charger.
 * @author Muhsin
 * @date 2026-04-27
 */

#include "logic.h"
#include "app_config.h"
#include "esp_log.h"

static const char *TAG = "LOGIC";

static system_state_t state = STATE_IDLE;
static uint8_t battery_state = 0;     // 0–100%
static uint8_t charging_mode = 0;     // 0=idle,1=precharge,2=charging,3=float,4=fault

// Hysteresis thresholds
#define PRECHARGE_EXIT_V   (PRECHARGE_VOLTAGE - 0.2f)
#define CHARGING_EXIT_V    (FLOAT_VOLTAGE - 0.2f)

/**
 * @brief Initialise logic module.
 */
void logic_init(void)
{
    state = STATE_IDLE;
    battery_state = 0;
    charging_mode = 0;
    ESP_LOGI(TAG, "Logic initialised (IDLE)");
}

/**
 * @brief Main logic step.
 */
void logic_step(safety_fault_t fault, const measurements_t *m)
{
    if (fault != SAFETY_OK)
    {
        if (state != STATE_FAULT)
            ESP_LOGW(TAG, "FAULT detected → entering FAULT state");

        state = STATE_FAULT;
        charging_mode = 4;
        return;
    }

    system_state_t prev = state;

    switch (state)
    {
        case STATE_IDLE:
            if (m->voltage > 0.5f)
                state = STATE_PRECHARGE;
            break;

        case STATE_PRECHARGE:
            if (m->voltage > PRECHARGE_VOLTAGE)
                state = STATE_CHARGING;
            else if (m->voltage < PRECHARGE_EXIT_V)
                state = STATE_IDLE;   // hysteresis
            break;

        case STATE_CHARGING:
            if (m->voltage > FLOAT_VOLTAGE)
                state = STATE_FLOAT;
            else if (m->voltage < CHARGING_EXIT_V)
                state = STATE_PRECHARGE;  // hysteresis
            break;

        case STATE_FLOAT:
            // remain in float unless voltage collapses
            if (m->voltage < FLOAT_VOLTAGE - 0.3f)
                state = STATE_CHARGING;
            break;

        case STATE_FAULT:
            // latched until reset
            break;
    }

    if (state != prev)
        ESP_LOGI(TAG, "State transition: %d → %d", prev, state);

    // -------------------------
    // Battery percentage estimate
    // -------------------------
    float pct = (m->voltage / FLOAT_VOLTAGE) * 100.0f;
    if (pct < 0) pct = 0;
    if (pct > 100) pct = 100;
    battery_state = (uint8_t)pct;

    // -------------------------
    // Charging mode reporting
    // -------------------------
    switch (state)
    {
        case STATE_IDLE:       charging_mode = 0; break;
        case STATE_PRECHARGE:  charging_mode = 1; break;
        case STATE_CHARGING:   charging_mode = 2; break;
        case STATE_FLOAT:      charging_mode = 3; break;
        case STATE_FAULT:      charging_mode = 4; break;
    }
}

/**
 * @brief Get current system state.
 */
system_state_t logic_get_state(void)
{
    return state;
}

uint8_t logic_get_battery_state(void)
{
    return battery_state;
}

uint8_t logic_get_charging_mode(void)
{
    return charging_mode;
}
