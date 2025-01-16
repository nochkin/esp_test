#ifdef PIOENV_esp32_4848s040

#include <Arduino.h>
#include <lvgl.h>
#include <demos/lv_demos.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

#include "esp32_4848s040.h"
#include "common_tasks_esp32.h"

class LGFX_4848s040 : public lgfx::LGFX_Device {
    lgfx::Bus_RGB      _bus_instance;
    lgfx::Panel_ST7701_guition_esp32_4848S040 _panel_instance;
    lgfx::Touch_GT911  _touch_instance;
    lgfx::Light_PWM    _light_instance;

public:
    LGFX_4848s040() {
        {
            auto cfg = _panel_instance.config();

            cfg.memory_width  = LV_HOR_RES;
            cfg.memory_height = LV_VER_RES;
            cfg.panel_width  = LV_HOR_RES;
            cfg.panel_height = LV_VER_RES;

            cfg.offset_x = 0;
            cfg.offset_y = 0;

            _panel_instance.config(cfg);
        }

        {
            auto cfg = _panel_instance.config_detail();

            cfg.pin_cs = 39;
            cfg.pin_sclk = 48;
            cfg.pin_mosi = 47; // SDA

            _panel_instance.config_detail(cfg);
        }

        {
            auto cfg = _bus_instance.config();
            cfg.panel = &_panel_instance;
            cfg.pin_d0  = GPIO_NUM_4;  // B0
            cfg.pin_d1  = GPIO_NUM_5;  // B1
            cfg.pin_d2  = GPIO_NUM_6; // B2
            cfg.pin_d3  = GPIO_NUM_7; // B3
            cfg.pin_d4  = GPIO_NUM_15;  // B4
            cfg.pin_d5  = GPIO_NUM_8;  // G0
            cfg.pin_d6  = GPIO_NUM_20;  // G1
            cfg.pin_d7  = GPIO_NUM_3; // G2
            cfg.pin_d8  = GPIO_NUM_46; // G3
            cfg.pin_d9  = GPIO_NUM_9; // G4
            cfg.pin_d10 = GPIO_NUM_10;  // G5
            cfg.pin_d11 = GPIO_NUM_11; // R0
            cfg.pin_d12 = GPIO_NUM_12; // R1
            cfg.pin_d13 = GPIO_NUM_13; // R2
            cfg.pin_d14 = GPIO_NUM_14; // R3
            cfg.pin_d15 = GPIO_NUM_0; // R4

            cfg.pin_henable = GPIO_NUM_18;
            cfg.pin_vsync   = GPIO_NUM_17;
            cfg.pin_hsync   = GPIO_NUM_16;
            cfg.pin_pclk    = GPIO_NUM_21;
            cfg.freq_write  = 14 * 1000 * 1000;

            cfg.hsync_polarity    = 0;
            cfg.hsync_front_porch = 10;
            cfg.hsync_pulse_width = 8;
            cfg.hsync_back_porch  = 50;
            cfg.vsync_polarity    = 0;
            cfg.vsync_front_porch = 10;
            cfg.vsync_pulse_width = 8;
            cfg.vsync_back_porch  = 20;
            cfg.pclk_idle_high    = 0;
            cfg.de_idle_high      = 1;
            _bus_instance.config(cfg);
        }
        _panel_instance.setBus(&_bus_instance);

        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = GPIO_NUM_38;
            _light_instance.config(cfg);
        }
        _panel_instance.light(&_light_instance);

        {
            auto cfg = _touch_instance.config();
            cfg.x_min      = 0;
            cfg.x_max      = 480;
            cfg.y_min      = 0;
            cfg.y_max      = 480;
            cfg.bus_shared = false;
            cfg.offset_rotation = 0;

            cfg.i2c_port   = I2C_NUM_1;

            cfg.pin_int    = GPIO_NUM_NC;
            cfg.pin_sda    = GPIO_NUM_19;
            cfg.pin_scl    = GPIO_NUM_45;
            cfg.pin_rst    = GPIO_NUM_NC;

            cfg.freq       = 400000;
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }

        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = GPIO_NUM_38;
            _light_instance.config(cfg);
        }
        _panel_instance.light(&_light_instance);

        setPanel(&_panel_instance);
    }
};

LGFX_4848s040 tft;
uint32_t lv_width = LV_HOR_RES;
uint32_t lv_height = LV_VER_RES;
uint8_t disp_buf[480 * 100 * sizeof(lv_color_t)];

static void disp_flush(lv_display_t *disp, const lv_area_t *area, unsigned char *color_p) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    // lv_draw_sw_rgb565_swap(color_p, w*h);
    tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t *)color_p);
    lv_display_flush_ready(disp);
}

static void touchpad_read(lv_indev_t * indev_drv, lv_indev_data_t * data) {
    uint16_t touchX, touchY;
    data->state = LV_INDEV_STATE_RELEASED;
    uint8_t touches = tft.getTouch(&touchX, &touchY);
    if (touches > 0) {
        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = touchX;
        data->point.y = touchY;
        // Serial.printf("Touch: (%03d,%03d)\n", data->point.x, data->point.y);
    }
}

void show_lv_mem() {
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    log_d("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n", (int)mon.total_size - mon.free_size, mon.used_pct, mon.frag_pct, (int)mon.free_biggest_size);
}

static uint32_t my_tick(void) {
    return millis();
}

void loop_task(void *task) {
    log_d("LVGL loop task");
    while (1) {
        uint32_t tick = lv_timer_handler();
        // lv_tick_inc(tick);
        vTaskDelay((tick > 0 ? tick : 1) / portTICK_PERIOD_MS);
    }
}

static void rotate(lv_display_t *disp_drv) {
    tft.setRotation(1);
    lv_display_set_rotation(disp_drv, LV_DISPLAY_ROTATION_90);
    lv_width = LV_VER_RES;
    lv_height = LV_HOR_RES;

}

static void my_lvgl_init() {
    tft.init();
    lv_init();

    tft.setBrightness(128);

    lv_display_t *disp_drv = lv_display_create(lv_width, lv_height);
    lv_display_set_flush_cb(disp_drv, disp_flush);
    lv_display_set_buffers(disp_drv, disp_buf, NULL, sizeof(disp_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev_drv = lv_indev_create();
    lv_indev_set_type(indev_drv, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_drv, touchpad_read);

    // rotate(disp_drv);
    // lv_theme_t *lv_theme = lv_theme_default_init(disp_drv, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREY), true, LV_FONT_DEFAULT);
    // lv_disp_set_theme(disp_drv, lv_theme);

    lv_tick_set_cb(my_tick);
}

void setup_custom() {
    my_lvgl_init();
    // lv_demo_music();
    lv_demo_widgets();
    // lv_demo_benchmark();

    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(loop_task, "loop", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
}

void loop_custom() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

#endif
