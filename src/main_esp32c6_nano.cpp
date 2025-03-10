#if defined(PIOENV_esp32c6_n8_nano) || defined(PIOENV_esp32c6_n16_nano)

#include <Arduino.h>

#include "esp32c6_nano.h"
#include "common_tasks_esp32.h"

void setup_custom() {
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(ws2812_task, "ws2812", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void*)MY_WS2812_PIN, 10, NULL);
}

void loop_custom() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
