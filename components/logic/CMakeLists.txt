idf_component_register(
    SRCS "logic.c"
    INCLUDE_DIRS "."
    REQUIRES safety app_config esp_adc
)
