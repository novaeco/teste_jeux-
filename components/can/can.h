/*****************************************************************************
 * | File         :   can.h
 * | Author       :   Waveshare team
 * | Function     :   CAN driver code for CAN communication
 * | Info         :
 * |                 This header file contains definitions and function
 * |                 declarations for CAN communication using the IO EXTENSION chip
 * |                 and ESP-IDF TWAI driver.
 * ----------------
 * | This version :   V1.0
 * | Date         :   2024-11-28
 * | Info         :   Basic version
 *
 ******************************************************************************/

#ifndef __CAN_H
#define __CAN_H

// Include necessary driver headers
#include <stdbool.h>
#include "driver/twai.h"   // TWAI (CAN) driver
#include "io_extension.h"        // IO EXTENSION I2C CAN control header

// Alias type for CAN messages used by the application
typedef twai_message_t can_message_t;

// Define the GPIO pins used for CAN communication
#define TX_GPIO_NUM GPIO_NUM_20 // Transmit GPIO number for CAN
#define RX_GPIO_NUM GPIO_NUM_19 // Receive GPIO number for CAN
#define CAN_TAG "TWAI Master" // Log tag for the CAN example (for logging)

// Define transmission and polling intervals in milliseconds
#define TRANSMIT_RATE_MS 1000 // Transmission interval for CAN messages
#define POLLING_RATE_MS 1000  // Polling interval for checking CAN status

// Function declarations:

/**
 * @brief Initializes the CAN (TWAI) interface with the provided configurations.
 *
 * @param t_config Timing configuration for CAN communication.
 * @param f_config Filter configuration for CAN.
 * @param g_config General configuration for the CAN interface.
 *
 * @return ESP_OK on success, or an error code on failure.
 */
esp_err_t can_init(twai_timing_config_t t_config, twai_filter_config_t f_config, twai_general_config_t g_config);

/**
 * @brief Reads and returns any alerts from the CAN interface.
 *
 * @return A 32-bit value representing the alerts.
 */
uint32_t can_read_alerts();

/**
 * @brief Sends a single byte of data over the CAN interface.
 *
 * @param message The CAN message to be transmitted.
 */
void can_write_Byte(can_message_t message);

/**
 * @brief Reads a single byte of data from the CAN interface.
 *
 * @return A CAN message containing the received data.
 */
can_message_t can_read_Byte();

bool can_is_active(void);

#endif  // __CAN_H
