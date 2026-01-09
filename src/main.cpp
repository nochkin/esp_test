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
#elif PIOENV_esp32c6_n8_nano
#include "esp32c6_nano.h"
#elif PIOENV_esp32c6_n16_nano
#include "esp32c6_nano.h"
#elif PIOENV_esp32s3_supermini
#include "esp32s3_supermini.h"
#elif PIOENV_esp32s3_n16r8
#include "esp32s3_n16r8.h"
#elif PIOENV_esp32s3_n8r2
#include "esp32s3_n8r2.h"
#elif PIOENV_esp32_4848s040
#include "esp32_4848s040.h"
#elif PIOENV_esp32_8048s043
#include "esp32_8048s043.h"
#elif PIOENV_esp32c3_042_oled
#include "esp32c3_042_oled.h"
#elif PIOENV_esp32s3_ai
#include "esp32s3_ai.h"
#elif PIOENV_sp_esp32s3_nano
#include "sp_esp32s3_nano.h"
#elif PIOENV_ws_esp32s3_pico_n16r2
#include "ws_esp32s3_pico.h"
#elif PIOENV_white_esp32_s3_nano_n16r8
#include "white_esp32_s3_nano.h"
#else
void setup_custom() {}
void loop_custom() {}
#endif

void setup()
{
    delay(1000);
    Serial.begin(115200);
    Serial.println("");
    Serial.println("Start");
    setup_custom();
}

void loop()
{
    loop_custom();
}
