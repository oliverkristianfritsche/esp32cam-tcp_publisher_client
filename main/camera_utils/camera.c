#include "esp_log.h"
#include <esp_system.h>
#include <nvs_flash.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "camera_pins.h"
#include "esp_camera.h"
#include "image_publisher.h"
#include "config.h"

// Initialize the camera configuration structure
camera_config_t camera_config = {
    .pin_pwdn  = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz   = CAM_XCLK_FREQ_HZ,
    .pixel_format   = CAM_PIXEL_FORMAT,
    .frame_size     = CAM_FRAME_SIZE,
    .jpeg_quality   = CAM_JPEG_QUALITY,
    .fb_count       = CAM_FB_COUNT,
};

esp_err_t err;
camera_fb_t* fb;

void camera_initialize(){
    err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE("MAIN", "Camera init failed with error: 0x%x", err);
        return;
    }
}

camera_fb_t* camera_get_frame(){
    fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE("MAIN", "Camera capture failed");
    }
    return fb;
}

void camera_release_frame(){
    esp_camera_fb_return(fb);
}

void capture_frame(){
    camera_initialize();
    while(1){
        fb = camera_get_frame();
        if (fb) {
            send_frame(fb);
            camera_release_frame();
        }
    }
}