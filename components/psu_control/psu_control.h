#pragma once

#include <stdint.h>

typedef struct {
    float voltage;
    float current;
} psu_state_t;

void psu_control_init(void);
void psu_control_update(psu_state_t *state);
