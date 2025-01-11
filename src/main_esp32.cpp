#ifdef PIOENV_esp32

#include <Arduino.h>

#include "esp32.h"
#include "common_tasks_esp32.h"

void setup_custom() {
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
}

void loop_custom() {
    delay(1);
}

#endif
