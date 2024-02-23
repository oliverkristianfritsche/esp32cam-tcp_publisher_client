// servo.h
#ifndef CAMERA_H
#define CAMERA_H

#include "esp_camera.h"
//initial mcpwm configuration

void capture_frame(void);

void camera_initialize(void);

camera_fb_t* camera_get_frame();

void camera_release_frame();
#endif // CAMERA_H
