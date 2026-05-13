idf_component_register(
    SRCS "app.c"
    INCLUDE_DIRS "."
    REQUIRES measurement safety logic output app_config esp_adc can_driver can_protocol psu_can_tx
)
