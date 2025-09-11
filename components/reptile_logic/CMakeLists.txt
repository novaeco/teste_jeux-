idf_component_register(
    SRCS "reptile_logic.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash gpio
    PRIV_REQUIRES sensors
)
