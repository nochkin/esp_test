#ifdef PIOENV_esp32c3_supermini

#include <Arduino.h>

#include "esp32c3_supermini.h"
#include "common_tasks_esp32.h"

void setup_custom() {
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(led_task, "led", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void*)MY_LED_PIN, 10, NULL);
}

void loop_custom() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
