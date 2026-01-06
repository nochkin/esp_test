#ifdef PIOENV_white_esp32_s3_nano_n16r8

#include <Arduino.h>
#include <FreeRTOS.h>
#include <FastLED.h>

#include "white_esp32_s3_nano.h"
#include "common_tasks_esp32.h"

void setup_custom() {
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(led_task, "led", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void*)MY_LED_PIN, 10, NULL);
}

void loop_custom() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
