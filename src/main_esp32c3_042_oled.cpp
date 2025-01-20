#ifdef PIOENV_esp32c3_042_oled

#include <Arduino.h>
#include <U8g2lib.h>

// #define LGFX_USE_V1
// #include <LovyanGFX.hpp>

#include "esp32c3_042_oled.h"
#include "common_tasks_esp32.h"

/*
class LGFX_SSD1306 : public lgfx::LGFX_Device {
    lgfx::Panel_SSD1306 _panel_instance;
    lgfx::Bus_I2C       _bus_instance;

public:
    LGFX_SSD1306(void) {
        {
            auto cfg = _bus_instance.config();
            cfg.i2c_port = 0;
            cfg.freq_write = 400 * 1000;
            cfg.pin_scl = 6;
            cfg.pin_sda = 5;
            cfg.i2c_addr = 0x3c;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();
            cfg.pin_rst = -1;
            cfg.memory_width = 132;
            cfg.memory_height = 64;
            cfg.panel_width = 72;
            cfg.panel_height = 40;
            cfg.offset_x = 30;
            cfg.offset_y = 12;
            cfg.offset_rotation = 0;
            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance);
    }
};

LGFX_SSD1306 tft;
*/

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5); // rotation, reset, clock, data

static void lcd042_task(void *task) {
    int width = 72;
    int height = 40;
    int xOffset = 30; // = (132-w)/2
    int yOffset = 12; // = (64-h)/2

    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.setBusClock(400 * 1000);
    u8g2.setFont(u8g2_font_ncenB10_tr);

    uint32_t count = 0;
    while (true) {
        u8g2.clearBuffer();
        u8g2.drawFrame(xOffset+0, yOffset+0, width, height);
        u8g2.setCursor(xOffset+20, yOffset+25);
        // u8g2.printf("%dx%d", width, height);
        u8g2.printf("%04X", count);
        u8g2.sendBuffer();
        count++;
        vTaskDelay(300);
    }

    vTaskDelete(NULL);
}

void setup_custom() {
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(led_task, "led", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED_PIN, 10, NULL);
    xTaskCreate(lcd042_task, "lcd", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);

    /*
    i2c_cfg_t i2c_cfg;
    i2c_cfg.sda = 5;
    i2c_cfg.scl = 6;
    i2c_cfg.freq = 100 * 1000;
    xTaskCreate(i2c_scan_task, "i2c_scan", CONFIG_ARDUINO_LOOP_STACK_SIZE, &i2c_cfg, 10, NULL);
    */

    /*
    tft.init();
    tft.clear();
    tft.setBrightness(255);
    // tft.drawRect(0, 10, 80, 40, TFT_WHITE);
    tft.fillScreen(TFT_WHITE);
    */
}

void loop_custom() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
