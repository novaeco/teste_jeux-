idf_component_register(
    SRCS "reptile_logic.c" "env_control.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash gpio
    PRIV_REQUIRES sensors
)
