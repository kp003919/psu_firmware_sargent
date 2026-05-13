#include "log_sys.h"
#include "esp_log.h"

static const char *TAG = "LOG_SYS";

void log_sys_init(void)
{
    ESP_LOGI(TAG, "Logging system initialized");
}

void log_sys_info(const char *msg)
{
    ESP_LOGI(TAG, "%s", msg);
}
