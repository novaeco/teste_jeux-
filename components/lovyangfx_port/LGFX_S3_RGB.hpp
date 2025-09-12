#pragma once

#include <LovyanGFX.hpp>
#include "rgb_lcd_port.h"

class LGFX_S3_RGB : public lgfx::LGFX_Device {
    lgfx::Bus_RGB _bus;
    lgfx::Panel_RGB _panel;
public:
    LGFX_S3_RGB();
};

extern LGFX_S3_RGB gfx;
