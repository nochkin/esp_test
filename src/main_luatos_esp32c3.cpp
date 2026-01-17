#ifdef PIOENV_luatos_esp32c3

#include <Arduino.h>

#include "luatos_esp32c3.h"
#include "common_tasks_esp32.h"

void setup_custom()
{
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(led_task, "led1", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED1_PIN, 10, NULL);
    vTaskDelay(500 / portTICK_PERIOD_MS); // Delay to stagger LED tasks
    xTaskCreate(led_task, "led2", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED2_PIN, 10, NULL);
}

void loop_custom()
{
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
