#pragma once

#define LGFX_USE_V1
#define LGFX_RGB_PARALLEL
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32/Bus_RGB.hpp>
#include <lgfx/v1/platforms/esp32/Panel_RGB.hpp>
#include "rgb_lcd_port.h"

class LGFX_S3_RGB : public lgfx::LGFX_Device {
    lgfx::v1::Bus_RGB _bus;
    lgfx::v1::Panel_RGB _panel;
public:
    LGFX_S3_RGB();
};

extern LGFX_S3_RGB gfx;
