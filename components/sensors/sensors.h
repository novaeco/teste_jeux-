#ifndef SENSORS_H
#define SENSORS_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t sensors_init(void);
float sensors_read_temperature(void);
float sensors_read_humidity(void);

#ifdef __cplusplus
}
#endif

#endif // SENSORS_H
