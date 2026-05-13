idf_component_register(
    SRCS "psu_can_tx.c"
    INCLUDE_DIRS "."
    REQUIRES can_driver can_protocol measurement logic output
)
