/**
 * @file can_driver.h
 * @brief Public API for CAN (TWAI) driver with simulation support.
 */

#pragma once

#include "esp_err.h"
#include "driver/twai.h"

/**
 * @brief Initialise the CAN driver.
 *
 * In SIMULATION mode:
 *   - No hardware is initialised
 *   - No TWAI driver is installed
 *   - Always returns ESP_OK
 *
 * In REAL mode:
 *   - Installs TWAI driver
 *   - Starts CAN controller
 *
 * @return ESP_OK on success, or TWAI error code.
 */
esp_err_t can_driver_init(void);


/**
 * @brief Transmit a CAN frame.
 *
 * In SIMULATION mode:
 *   - Logs the frame
 *   - Always returns ESP_OK
 *
 * In REAL mode:
 *   - Calls twai_transmit()
 *   - Returns ESP_ERR_TIMEOUT if no ACK on the bus
 *
 * @param msg Pointer to a TWAI message structure.
 * @return ESP_OK on success, or TWAI error code.
 */
esp_err_t can_driver_transmit(const twai_message_t *msg);


/**
 * @brief Receive a CAN frame.
 *
 * In SIMULATION mode:
 *   - Always returns ESP_ERR_TIMEOUT (no messages)
 *
 * In REAL mode:
 *   - Calls twai_receive()
 *
 * @param msg Pointer to TWAI message structure to fill.
 * @return ESP_OK on success, ESP_ERR_TIMEOUT if no message.
 */
esp_err_t can_driver_receive(twai_message_t *msg);


/**
 * @brief Shut down the CAN driver.
 *
 * In SIMULATION mode:
 *   - Logs shutdown
 *
 * In REAL mode:
 *   - Stops TWAI driver
 *   - Uninstalls TWAI driver
 *
 * @return ESP_OK
 */
esp_err_t can_driver_shutdown(void);
