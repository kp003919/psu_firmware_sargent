#include "unity.h"
#include "can_parser.h"

void setUp(void) {}
void tearDown(void) {}

void test_can_parser_runs(void)
{
    can_parser_init();
    can_parser_process(0x100, NULL, 0);
    TEST_PASS();
}
