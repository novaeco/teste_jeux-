idf_component_register(SRCS "sensors.c"
                        INCLUDE_DIRS "."
                        REQUIRES i2c freertos config esp_system)
