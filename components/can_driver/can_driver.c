/**
 * @file can_driver.c
 * @brief CAN (TWAI) driver with simulation mode support.
 */

#include "can_driver.h"
#include "esp_log.h"
#include "driver/twai.h"
#include <inttypes.h>


// =========================================================
//  CONFIGURATION
// =========================================================
#define CAN_SIMULATION_MODE   1   // 1 = simulate, 0 = real CAN

static const char *TAG = "CAN_DRIVER";


// =========================================================
//  INITIALISATION
// =========================================================
esp_err_t can_driver_init(void)
{
#if CAN_SIMULATION_MODE
    ESP_LOGW(TAG, "CAN driver running in SIMULATION MODE (no hardware required)");
    return ESP_OK;

#else
    ESP_LOGI(TAG, "Initialising REAL CAN driver...");

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NORMAL);
    twai_timing_config_t  t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t  f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    esp_err_t err;

    err = twai_driver_install(&g_config, &t_config, &f_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install TWAI driver: %s", esp_err_to_name(err));
        return err;
    }

    err = twai_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start TWAI driver: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "REAL CAN driver initialised");
    return ESP_OK;
#endif
}



// =========================================================
//  TRANSMIT
// =========================================================
esp_err_t can_driver_transmit(const twai_message_t *msg)
{
#if CAN_SIMULATION_MODE
    ESP_LOGI(TAG, "[SIM] TX ID=0x%03" PRIx32 " DLC=%d",
         msg->identifier,
         msg->data_length_code);

    return ESP_OK;

#else
    esp_err_t err = twai_transmit(msg, pdMS_TO_TICKS(10));

    if (err != ESP_OK) {
        ESP_LOGW(TAG, "CAN TX failed: %s", esp_err_to_name(err));
    }

    return err;
#endif
}



// =========================================================
//  RECEIVE (optional)
// =========================================================
esp_err_t can_driver_receive(twai_message_t *msg)
{
#if CAN_SIMULATION_MODE
    // No incoming messages in simulation
    return ESP_ERR_TIMEOUT;

#else
    return twai_receive(msg, pdMS_TO_TICKS(10));
#endif
}



// =========================================================
//  SHUTDOWN
// =========================================================
esp_err_t can_driver_shutdown(void)
{
#if CAN_SIMULATION_MODE
    ESP_LOGI(TAG, "[SIM] CAN shutdown");
    return ESP_OK;

#else
    ESP_LOGI(TAG, "Stopping CAN driver...");
    twai_stop();
    twai_driver_uninstall();
    return ESP_OK;
#endif
}
