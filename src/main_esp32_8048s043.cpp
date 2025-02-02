#ifdef PIOENV_esp32_8048s043

#include <Arduino.h>
#include <lvgl.h>
#include <demos/lv_demos.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

// #include <Arduino_GFX_Library.h>

#include "esp32_8048s043.h"
#include "common_tasks_esp32.h"

class LGFX_8048s043 : public lgfx::LGFX_Device {
    lgfx::Bus_RGB      _bus_instance;
    lgfx::Panel_RGB    _panel_instance;
    lgfx::Touch_GT911  _touch_instance;
    lgfx::Light_PWM    _light_instance;

public:
    LGFX_8048s043() {
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

            // cfg.use_psram = 1;

            _panel_instance.config_detail(cfg);
        }

        {
            auto cfg = _bus_instance.config();
            cfg.panel = &_panel_instance;
            cfg.pin_d0  = GPIO_NUM_8;  // B0
            cfg.pin_d1  = GPIO_NUM_3;  // B1
            cfg.pin_d2  = GPIO_NUM_46; // B2
            cfg.pin_d3  = GPIO_NUM_9; // B3
            cfg.pin_d4  = GPIO_NUM_1;  // B4
            cfg.pin_d5  = GPIO_NUM_5;  // G0
            cfg.pin_d6  = GPIO_NUM_6;  // G1
            cfg.pin_d7  = GPIO_NUM_7; // G2
            cfg.pin_d8  = GPIO_NUM_15; // G3
            cfg.pin_d9  = GPIO_NUM_16; // G4
            cfg.pin_d10 = GPIO_NUM_4;  // G5
            cfg.pin_d11 = GPIO_NUM_45; // R0
            cfg.pin_d12 = GPIO_NUM_48; // R1
            cfg.pin_d13 = GPIO_NUM_47; // R2
            cfg.pin_d14 = GPIO_NUM_21; // R3
            cfg.pin_d15 = GPIO_NUM_14; // R4

            cfg.pin_henable = GPIO_NUM_40; // DE
            cfg.pin_vsync   = GPIO_NUM_41;
            cfg.pin_hsync   = GPIO_NUM_39;
            cfg.pin_pclk    = GPIO_NUM_42;

            cfg.hsync_polarity    = 0;
            cfg.hsync_front_porch = 8;
            cfg.hsync_pulse_width = 4;
            cfg.hsync_back_porch  = 8;
            cfg.vsync_polarity    = 0;
            cfg.vsync_front_porch = 8;
            cfg.vsync_pulse_width = 4;
            cfg.vsync_back_porch  = 8;
            cfg.pclk_idle_high    = 1;
            cfg.freq_write        = 16 * 1000 * 1000;
            _bus_instance.config(cfg);
        }
        _panel_instance.setBus(&_bus_instance);

        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = GPIO_NUM_2;
            cfg.invert = false;
            cfg.freq = 44100;
            cfg.pwm_channel = 2;
            _light_instance.config(cfg);
        }
        // _panel_instance.setLight(&_light_instance);

        {
            auto cfg = _touch_instance.config();
            cfg.x_min      = 0;
            cfg.x_max      = 480;
            cfg.y_min      = 0;
            cfg.y_max      = 272; 
            cfg.bus_shared = false;
            cfg.offset_rotation = 0;

            cfg.i2c_port   = I2C_NUM_1;

            // cfg.pin_int    = GPIO_NUM_18;
            cfg.pin_sda    = GPIO_NUM_19;
            cfg.pin_scl    = GPIO_NUM_20;
            cfg.pin_rst    = GPIO_NUM_38;

            cfg.freq       = 400 * 1000;
            cfg.i2c_addr   = 0x5d; // 0x5d, 0x14
            _touch_instance.config(cfg);
        }
        _panel_instance.setTouch(&_touch_instance);

        setPanel(&_panel_instance);
    }
};

LGFX_8048s043 tft;

//Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
//    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
//    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
//    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
//    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
//    0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
//    0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
//    1 , 14 * 1000 * 1000, false, 1, 1
//);

//Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//    800 /* width */, 480 /* height */, bus, 0 /* rotation */, true /* auto_flush */);

#define LVGL_BUF_SIZE LV_HOR_RES * 60 * sizeof(lv_color_t)
uint8_t disp_buf[LVGL_BUF_SIZE];
// lv_color_t *disp_buf = (lv_color_t *)gfx->getFramebuffer();
// lv_color_t *disp_buf = (lv_color_t*) heap_caps_malloc(LVGL_BUF_SIZE, MALLOC_CAP_INTERNAL);
// lv_color_t *disp_buf = (lv_color_t*) heap_caps_malloc(LVGL_BUF_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
// lv_color_t *disp_buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_SIZE, MALLOC_CAP_SPIRAM);
// static lv_color_t *disp_buf = (lv_color_t *)heap_caps_malloc(LVGL_BUF_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

uint32_t lv_width = LV_HOR_RES;
uint32_t lv_height = LV_VER_RES;

static void disp_flush(lv_display_t *disp, const lv_area_t *area, unsigned char *color_p) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    // lv_draw_sw_rgb565_swap(color_p, w*h);
    tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t *)color_p);

    // gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)color_p, w, h);
    lv_disp_flush_ready(disp);
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
    // tft.setRotation(1);
    lv_display_set_rotation(disp_drv, LV_DISPLAY_ROTATION_90);
    lv_width = LV_VER_RES;
    lv_height = LV_HOR_RES;

}

static void my_lvgl_init() {
    analogWrite(2, 120);
    tft.init();
    // gfx->begin();

    lv_init();

    // tft.setBrightness(250);
    analogWrite(2, 120);

    lv_display_t *disp_drv = lv_display_create(lv_width, lv_height);
    lv_display_set_flush_cb(disp_drv, disp_flush);
    lv_display_set_buffers(disp_drv, disp_buf, NULL, LVGL_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

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
    lv_demo_music();
    // lv_demo_widgets();
    // lv_demo_benchmark();

    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(loop_task, "loop", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);

    /*
    i2c_cfg_t i2c_cfg;
    i2c_cfg.sda = 19;
    i2c_cfg.scl = 20;
    i2c_cfg.freq = 100 * 1000;
    xTaskCreate(i2c_scan_task, "i2c_scan", CONFIG_ARDUINO_LOOP_STACK_SIZE, &i2c_cfg, 10, NULL);
    */
}

void loop_custom() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

#endif
