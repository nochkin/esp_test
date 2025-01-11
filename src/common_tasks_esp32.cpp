#ifdef ESP32

#include <Arduino.h>
#include <FastLED.h>

void ws2812_task(void *task) {
    uint8_t led_pin = (uint32_t)task & 0xff;
    uint8_t hue = 0;
    int8_t hue_step = 1;
    CRGB leds[1];

    Serial.printf("WS2812 task on GPIO %i\n", led_pin);

    switch (led_pin) {
#ifdef CONFIG_IDF_TARGET_ESP32S3
        case 48:
            FastLED.addLeds<WS2812, 48>(leds, 1);
            break;
#endif
#ifdef CONFIG_IDF_TARGET_ESP32C6
        case 8:
            FastLED.addLeds<WS2812, 8>(leds, 1);
            break;
#endif
        default:
            vTaskDelete(NULL);
            break;
    }

    while (true) {
        leds[0] = CHSV(hue, 255, 64);
        hue += hue_step;
        if (hue == 255 || hue == 0) { hue_step = -hue_step; }
        FastLED.show();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void led_task(void *task) {
    uint8_t led_pin = (uint32_t)task & 0xff;
    uint8_t brightness = 0;
    int8_t brightness_step = 1;

    Serial.printf("LED task on GPIO %i\n", led_pin);

    while (true) {
        analogWrite(led_pin, brightness);
        brightness += brightness_step;
        if (brightness == 255 || brightness == 0) { brightness_step = -brightness_step; }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

#endif
