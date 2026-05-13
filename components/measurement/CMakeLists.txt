idf_component_register(
    SRCS "measurement.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES driver app_config  esp_adc
)
