#pragma once

#include <stdint.h>

void can_parser_init(void);
void can_parser_process(uint32_t id, const uint8_t *data, uint8_t len);
