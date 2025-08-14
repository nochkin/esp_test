#ifdef PIOENV_esp32_tdisplay

#include <Arduino.h>
#include <lvgl.h>
#include <demos/lv_demos.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "esp32_tdisplay.h"
#include "common_tasks_esp32.h"

class LGFX_TDisplay : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;

public:
    LGFX_TDisplay()
    {
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host = VSPI_HOST;
            cfg.dma_channel = 1;
            cfg.spi_mode = 0;
            cfg.freq_write = MY_TFT_SPI_FREQ;
            cfg.freq_read = MY_TFT_SPI_FREQ / 4;
            cfg.spi_3wire = true;
            cfg.pin_sclk = MY_TFT_CLK;
            cfg.pin_mosi = MY_TFT_MOSI;
            cfg.pin_miso = -1;
            cfg.pin_dc = MY_TFT_DC;
            _bus_instance.config(cfg);
        }
        _panel_instance.setBus(&_bus_instance);

        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs = MY_TFT_CS;
            cfg.pin_rst = MY_TFT_RST;
            cfg.pin_busy = -1;
            // cfg.memory_width = LV_HOR_RES;
            // cfg.memory_height = LV_VER_RES;
            cfg.panel_width = LV_HOR_RES;
            cfg.panel_height = LV_VER_RES;
            cfg.offset_x = 52;
            cfg.offset_y = 40;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 16;
            cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = true;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = false;
            _panel_instance.config(cfg);

            setPanel(&_panel_instance);
        }

        {
            auto cfg = _light_instance.config();

            cfg.pin_bl = MY_TFT_BL;
            cfg.invert = false;
            cfg.freq = 44100;
            cfg.pwm_channel = 3;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }
    }
};

LGFX_TDisplay tft;
uint32_t lv_width = LV_HOR_RES;
uint32_t lv_height = LV_VER_RES;
uint8_t disp_buf[240 * 15 * sizeof(lv_color_t)];

static void disp_flush(lv_display_t *disp, const lv_area_t *area, unsigned char *color_p) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    // lv_draw_sw_rgb565_swap(color_p, w*h);
    tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t *)color_p);
    lv_display_flush_ready(disp);
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
    tft.setRotation(3);
    lv_display_set_rotation(disp_drv, LV_DISPLAY_ROTATION_270);
    lv_width = LV_VER_RES;
    lv_height = LV_HOR_RES;
}

static void my_lvgl_init() {
    tft.init();
    lv_init();

    tft.setBrightness(10);

    lv_display_t *disp_drv = lv_display_create(lv_width, lv_height);
    lv_display_set_flush_cb(disp_drv, disp_flush);
    lv_display_set_buffers(disp_drv, disp_buf, NULL, sizeof(disp_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

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
}

void loop_custom() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

#endif
