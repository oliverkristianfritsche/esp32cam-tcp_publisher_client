idf_component_register(SRCS "main.c" "camera_utils/camera.c" "wifi/wifi_manager.c" "tcp/image_publisher.c" "config.h"
                    INCLUDE_DIRS "." "camera_utils" "wifi" "tcp")