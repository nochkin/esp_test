#ifdef PIOENV_esp32s3_supermini

#include <Arduino.h>

#include "esp32s3_supermini.h"
#include "common_tasks.h"

void setup_custom() {
    xTaskCreate(ws2812_task, "ws2812", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void*)MY_WS2812_PIN, 10, NULL);
}

void loop_custom() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
