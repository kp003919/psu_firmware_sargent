idf_component_register(
    SRCS "psu_control.c" 
    INCLUDE_DIRS "."
    PRIV_REQUIRES measurement safety app  logic output app_config  
)

