#include <Arduino.h>

typedef struct
{
    uint8_t sda;
    uint8_t scl;
    uint32_t freq;
} i2c_cfg_t;

void ws2812_task(void *task);
void led_task(void *task);
void info_task(void *task);
void wifi_task(void *task);
void i2c_scan_task(void *task);

#if defined(PIOENV_esp32s3_cam)
#include "esp_camera.h"
void camera_task(void *task);
#endif
