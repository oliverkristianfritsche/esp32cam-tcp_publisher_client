idf_component_register(SRCS "main.c" "camera_utils/camera.c" "wifi/wifi_manager.c" "logger/logger.c" "tcp/image_publisher.c"
                    INCLUDE_DIRS "." "camera_utils" "wifi" "logger" "tcp")