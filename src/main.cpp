#include <Arduino.h>
#include <FastLED.h>

void setup_custom();
void loop_custom();

#ifdef PIOENV_esp8266
#elif PIOENV_esp32
#elif PIOENV_esp32_tdisplay
#include "esp32_tdisplay.h"
#elif PIOENV_esp32c3_supermini
#include "esp32c3_supermini.h"
#elif PIOENV_esp32c6_supermini
#include "esp32c6_supermini.h"
#elif PIOENV_esp32s3_supermini
#include "esp32s3_supermini.h"
#elif PIOENV_esp32s3_n16r8
#include "esp32s3_n16r8.h"
#elif PIOENV_esp32s3_n8r2
#include "esp32s3_n8r2.h"
#endif

void setup() {
    Serial.begin(115200);
    Serial.println("");
    Serial.println("Start");
    setup_custom();
}

void loop() {
    loop_custom();
}
