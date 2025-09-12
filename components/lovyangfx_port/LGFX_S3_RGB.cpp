#include "LGFX_S3_RGB.hpp"

LGFX_S3_RGB lgfx;

LGFX_S3_RGB::LGFX_S3_RGB()
{
    {
        auto cfg = _bus.config();
        cfg.panel = &_panel;
        cfg.freq_write = EXAMPLE_LCD_PIXEL_CLOCK_HZ;
        cfg.pin_pclk = EXAMPLE_LCD_IO_RGB_PCLK;
        cfg.pin_vsync = EXAMPLE_LCD_IO_RGB_VSYNC;
        cfg.pin_hsync = EXAMPLE_LCD_IO_RGB_HSYNC;
        cfg.pin_henable = EXAMPLE_LCD_IO_RGB_DE;
        cfg.pin_d0 = EXAMPLE_LCD_IO_RGB_DATA0;
        cfg.pin_d1 = EXAMPLE_LCD_IO_RGB_DATA1;
        cfg.pin_d2 = EXAMPLE_LCD_IO_RGB_DATA2;
        cfg.pin_d3 = EXAMPLE_LCD_IO_RGB_DATA3;
        cfg.pin_d4 = EXAMPLE_LCD_IO_RGB_DATA4;
        cfg.pin_d5 = EXAMPLE_LCD_IO_RGB_DATA5;
        cfg.pin_d6 = EXAMPLE_LCD_IO_RGB_DATA6;
        cfg.pin_d7 = EXAMPLE_LCD_IO_RGB_DATA7;
        cfg.pin_d8 = EXAMPLE_LCD_IO_RGB_DATA8;
        cfg.pin_d9 = EXAMPLE_LCD_IO_RGB_DATA9;
        cfg.pin_d10 = EXAMPLE_LCD_IO_RGB_DATA10;
        cfg.pin_d11 = EXAMPLE_LCD_IO_RGB_DATA11;
        cfg.pin_d12 = EXAMPLE_LCD_IO_RGB_DATA12;
        cfg.pin_d13 = EXAMPLE_LCD_IO_RGB_DATA13;
        cfg.pin_d14 = EXAMPLE_LCD_IO_RGB_DATA14;
        cfg.pin_d15 = EXAMPLE_LCD_IO_RGB_DATA15;
        cfg.hsync_pulse_width = 162;
        cfg.hsync_back_porch = 152;
        cfg.hsync_front_porch = 48;
        cfg.vsync_pulse_width = 45;
        cfg.vsync_back_porch = 13;
        cfg.vsync_front_porch = 3;
        cfg.pclk_active_neg = true;
        _bus.config(cfg);
    }

    {
        auto cfg = _panel.config();
        cfg.memory_width = EXAMPLE_LCD_H_RES;
        cfg.memory_height = EXAMPLE_LCD_V_RES;
        cfg.panel_width = EXAMPLE_LCD_H_RES;
        cfg.panel_height = EXAMPLE_LCD_V_RES;
        _panel.config(cfg);
    }

    _panel.setBus(&_bus);
    setPanel(&_panel);
    setColorDepth(16);
}
