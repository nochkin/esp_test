#ifdef PIOENV_esp32s3_cam

#include <Arduino.h>
#include <FreeRTOS.h>
#include <FastLED.h>

#include "esp32s3_cam.h"
#include "common_tasks_esp32.h"

void setup_custom()
{
    xTaskCreate(info_task, "info", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(wifi_task, "wifi", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 10, NULL);
    xTaskCreate(ws2812_task, "ws2812", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_WS2812_PIN, 10, NULL);
    xTaskCreate(led_task, "led1", CONFIG_ARDUINO_LOOP_STACK_SIZE, (void *)MY_LED1_PIN, 10, NULL);

    camera_config_t cam_config = {};
    cam_config.ledc_channel = LEDC_CHANNEL_0;
    cam_config.ledc_timer = LEDC_TIMER_0;
    cam_config.pin_d0 = MY_CAM_D0_PIN;
    cam_config.pin_d1 = MY_CAM_D1_PIN;
    cam_config.pin_d2 = MY_CAM_D2_PIN;
    cam_config.pin_d3 = MY_CAM_D3_PIN;
    cam_config.pin_d4 = MY_CAM_D4_PIN;
    cam_config.pin_d5 = MY_CAM_D5_PIN;
    cam_config.pin_d6 = MY_CAM_D6_PIN;
    cam_config.pin_d7 = MY_CAM_D7_PIN;
    cam_config.pin_xclk = MY_CAM_XCLK_PIN;
    cam_config.pin_pclk = MY_CAM_PCLK_PIN;
    cam_config.pin_vsync = MY_CAM_VSYNC_PIN;
    cam_config.pin_href = MY_CAM_HREF_PIN;
    cam_config.pin_sccb_sda = MY_CAM_SDA_PIN;
    cam_config.pin_sccb_scl = MY_CAM_SCL_PIN;
    cam_config.pin_pwdn = 21;
    cam_config.pin_reset = 22;
    cam_config.xclk_freq_hz = 10 * 1000 * 1000;
    cam_config.pixel_format = PIXFORMAT_JPEG;
    xTaskCreate(camera_task, "camera", CONFIG_ARDUINO_LOOP_STACK_SIZE, &cam_config, 10, NULL);
}

void loop_custom()
{
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

#endif
