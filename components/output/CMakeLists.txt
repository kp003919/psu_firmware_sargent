idf_component_register(
    SRCS "output.c"
    INCLUDE_DIRS "."
    REQUIRES logic app_config esp_driver_ledc  driver 
)
