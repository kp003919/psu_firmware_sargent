#include "psu_can_tx.h"
#include "can_driver.h"
#include <string.h>

void psu_can_send_electrical(const measurements_t *m)
{
    twai_message_t msg = {
        .identifier = CAN_ID_ELECTRICAL_STATUS,
        .data_length_code = 6,
        .rtr = 0,
        .extd = 0
    };

    msg.data[0] = (uint8_t)(m->voltage * 10.0f);   // 12.3V → 123
    msg.data[1] = (uint8_t)(m->current * 10.0f);   // 3.2A → 32
    msg.data[2] = (uint8_t)(m->temperature);       // °C

    msg.data[3] = 0;
    msg.data[4] = 0;
    msg.data[5] = 0;

    can_driver_transmit(&msg);
}

void psu_can_send_system_status(const system_status_t *s)
{
    twai_message_t msg = {
        .identifier = CAN_ID_SYSTEM_STATUS,
        .data_length_code = 4,
        .rtr = 0,
        .extd = 0
    };

    msg.data[0] = s->relay_state;     // 0/1
    msg.data[1] = s->pwm_percent;     // 0–100
    msg.data[2] = s->battery_state;   // enum
    msg.data[3] = s->charging_mode;   // enum

    can_driver_transmit(&msg);
}

void psu_can_send_tank_levels(const tank_levels_t *t)
{
    twai_message_t msg = {
        .identifier = CAN_ID_TANK_LEVELS,
        .data_length_code = 3,
        .rtr = 0,
        .extd = 0
    };

    msg.data[0] = t->fresh_percent;
    msg.data[1] = t->grey_percent;
    msg.data[2] = t->black_percent;

    can_driver_transmit(&msg);
}

void psu_can_send_fault_flags(uint8_t fault_mask)
{
    twai_message_t msg = {
        .identifier = CAN_ID_FAULT_FLAGS,
        .data_length_code = 1,
        .rtr = 0,
        .extd = 0
    };

    msg.data[0] = fault_mask;

    can_driver_transmit(&msg);
}
