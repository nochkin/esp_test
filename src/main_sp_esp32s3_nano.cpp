#ifdef PIOENV_sp_esp32s3_nano

#include <Arduino.h>
#include <FreeRTOS.h>
#include <FastLED.h>

#include "sp_esp32s3_nano.h"
#include "common_tasks_esp32.h"

void setup_custom()
{
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(led_task, "led", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED_PIN, 10, NULL);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    xTaskCreate(led_task, "led_r", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED_R_PIN, 10, NULL);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    xTaskCreate(led_task, "led_g", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED_G_PIN, 10, NULL);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    xTaskCreate(led_task, "led_b", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED_B_PIN, 10, NULL);
}

void loop_custom()
{
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
