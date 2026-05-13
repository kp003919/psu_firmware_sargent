idf_component_register(
    SRCS "safety.c"
    INCLUDE_DIRS "."
    REQUIRES measurement app_config esp_adc
)
