#ifdef ESP32

#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>

void ws2812_task(void *task) {
    uint8_t led_pin = (uint32_t)task & 0xff;
    uint8_t hue = 0;
    int8_t hue_step = 1;
    CRGB leds[1];

    log_d("WS2812 task on GPIO %i", led_pin);

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

    log_d("LED task on GPIO %i", led_pin);

    while (true) {
        analogWrite(led_pin, brightness);
        brightness += brightness_step;
        if (brightness == 255 || brightness == 0) { brightness_step = -brightness_step; }
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

void info_task(void *task) {
    uint64_t mac = ESP.getEfuseMac();
    log_d("MAC: %02X:%02X:%02X:%02X:%02X:%02X",
        (uint8_t)(mac >> 40),
        (uint8_t)(mac >> 32),
        (uint8_t)(mac >> 24),
        (uint8_t)(mac >> 16),
        (uint8_t)(mac >> 8),
        (uint8_t)(mac));

    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());

    vTaskDelete(NULL);
}

void wifi_task(void *task) {
    log_d("WiFi task");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    vTaskDelay(pdMS_TO_TICKS(1000));
    int16_t count = WiFi.scanNetworks();
    if (count == 0) {
        log_v("No WiFI APs found");
    } else {
        for (int16_t ii=0; ii<count; ii++) {
            log_v("%02d: %s (%d dBm) %s [BSSID: %s]",
                ii+1,
                WiFi.SSID(ii).c_str(),
                WiFi.RSSI(ii), 
                WiFi.encryptionType(ii) == WIFI_AUTH_OPEN ? "Open" : "Protected",
                WiFi.BSSIDstr(ii).c_str()
            );
        }
    }

    vTaskDelete(NULL);
}

#endif
