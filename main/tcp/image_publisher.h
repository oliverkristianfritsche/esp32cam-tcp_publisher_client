#include "esp_camera.h"
// im_stream.h
#ifndef IM_STREAM_H
#define IM_STREAM_H

esp_err_t send_frame(camera_fb_t *frame);
esp_err_t initialize_camera_stream();
#endif // IM_STREAM_H