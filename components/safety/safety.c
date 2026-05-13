#include "safety.h"
#include "app_config.h"   // thresholds

static safety_fault_t last_fault = SAFETY_OK;

void safety_init(void)
{
    last_fault = SAFETY_OK;
}

safety_fault_t safety_check(const measurements_t *m)
{
    if (m->voltage > MAX_VOLTAGE)
        last_fault = SAFETY_FAULT_OVERVOLT;
    else if (m->voltage < MIN_VOLTAGE)
        last_fault = SAFETY_FAULT_UNDERVOLT;
    else if (m->current > MAX_CURRENT)
        last_fault = SAFETY_FAULT_OVERCURRENT;
    else if (m->temperature > MAX_TEMPERATURE)
        last_fault = SAFETY_FAULT_OVERTEMP;
    else
        last_fault = SAFETY_OK;

    return last_fault;
}

bool safety_is_fault(safety_fault_t fault)
{
    return (fault != SAFETY_OK);
}
