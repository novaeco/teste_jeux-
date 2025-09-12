
idf_component_register(
    SRCS "lvgl_port.cpp" "lvgl_gpu_lgfx.cpp"
    INCLUDE_DIRS "."
    REQUIRES driver esp_lcd i2c gpio rgb_lcd_port touch lvgl LovyanGFX lovyangfx_port
)

                        