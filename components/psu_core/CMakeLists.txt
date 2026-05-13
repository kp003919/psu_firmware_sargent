idf_component_register(
    SRCS "psu_core.c" 
    INCLUDE_DIRS "."
    PRIV_REQUIRES measurement safety app  logic output app_config  
)

