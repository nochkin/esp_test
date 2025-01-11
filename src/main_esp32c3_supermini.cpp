#ifdef PIOENV_esp32c3_supermini

#include <Arduino.h>

#include "esp32c3_supermini.h"
#include "common_tasks.h"

void setup_custom() {
    xTaskCreate(led_task, "led", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void*)MY_LED_PIN, 10, NULL);
}

void loop_custom() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
