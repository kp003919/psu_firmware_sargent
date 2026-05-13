#pragma once
#include <stdint.h>

/* ============================================================
 *  CAN MESSAGE IDENTIFIERS
 * ============================================================ */
#define CAN_ID_ELECTRICAL_STATUS   0x120   // Voltage, current, temp
#define CAN_ID_SYSTEM_STATUS       0x121   // Relay, PWM, battery %, charging mode
#define CAN_ID_TANK_LEVELS         0x122   // Fresh/grey/black tank levels
#define CAN_ID_FAULT_FLAGS         0x123   // Fault bitmask


/* ============================================================
 *  ELECTRICAL STATUS FRAME (PSU → Panel)
 * ============================================================ */
typedef struct
{
    uint8_t voltage_x10;      // e.g., 12.3V → 123
    uint8_t current_x10;      // e.g., 3.2A → 32
    uint8_t temperature;      // °C
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
} can_electrical_status_t;


/* ============================================================
 *  SYSTEM STATUS FRAME (PSU → Panel)
 * ============================================================ */
typedef struct
{
    uint8_t relay_state;      // 0 = off, 1 = on
    uint8_t pwm_percent;      // 0–100
    uint8_t battery_state;    // 0–3 (your enum)
    uint8_t charging_mode;    // 0–5 (your enum)
} can_system_status_t;


/* ============================================================
 *  TANK LEVELS FRAME (PSU → Panel)
 * ============================================================ */
typedef struct
{
    uint8_t fresh_percent;    // 0–100%
    uint8_t grey_percent;     // 0–100%
    uint8_t black_percent;    // 0–100%
} can_tank_levels_t;


/* ============================================================
 *  FAULT FLAGS FRAME (PSU → Panel)
 * ============================================================ */
typedef struct
{
    uint8_t fault_mask;       // Bitmask: bit0=overcurrent, bit1=overtemp, etc.
} can_fault_flags_t;
