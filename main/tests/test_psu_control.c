#include "unity.h"
#include "psu_control.h"

void setUp(void) {}
void tearDown(void) {}

void test_psu_control_initializes(void)
{
    psu_control_init();
    TEST_PASS();
}
