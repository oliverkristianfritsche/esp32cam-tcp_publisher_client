#include "esp_camera.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "lwip/sockets.h"
#include "logger.h"

#define SERVER_IP   "192.168.3.132" // Change to your server's IP
#define SERVER_PORT 8082

static const char* TAG = "IMAGEPUBLISHER";

static int socket_fd = -1;
// Initialize the TCP connection
esp_err_t initialize_camera_stream() {
    struct sockaddr_in server_addr;

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (socket_fd < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
    }
    log_message(TAG, "Socket created", "INFO");
    ESP_LOGI(TAG, "Socket created");
    

    // Connect to server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr.s_addr);

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
        close(socket_fd);
        socket_fd = -1;
        return ESP_FAIL;
    }
    log_message(TAG, "Successfully connected", "INFO");
    ESP_LOGI(TAG, "Successfully connected");
    

    return ESP_OK;
}

esp_err_t send_frame(camera_fb_t *frame) {
    if (!frame) {
        log_message(TAG, "Frame is NULL", "ERROR");
        ESP_LOGE(TAG, "Frame is NULL");
        return ESP_FAIL;
    }

    if (socket_fd < 0) {
        log_message(TAG, "Socket is not initialized", "ERROR");
        ESP_LOGE(TAG, "Socket is not initialized");
        log_message(TAG, "Trying again to initialize socket in 5 seconds", "INFO");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        initialize_camera_stream();
        return ESP_FAIL;
    }

    // Send frame size first
    int frame_size = frame->len;
    // Convert frame size to network byte order
    int net_frame_size = htonl(frame_size);


    
    if (send(socket_fd, &net_frame_size, sizeof(net_frame_size), 0) < 0) {
        ESP_LOGE(TAG, "Error occurred during sending frame size: errno %d", errno);
        log_message(TAG, "Trying again to initialize socket in 5 seconds", "INFO");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        initialize_camera_stream();
        return ESP_FAIL;
    }

    // Send frame data
    int bytes_sent = send(socket_fd, frame->buf, frame->len, 0);
    if (bytes_sent < 0) {
        ESP_LOGE(TAG, "Error occurred during sending frame: errno %d", errno);
        log_message(TAG, "Trying again to initialize socket in 5 seconds", "INFO");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        initialize_camera_stream();
        return ESP_FAIL;
    }
    // log_message(TAG, "Frame sent");
    // ESP_LOGI(TAG, "Frame sent");

    return ESP_OK;
}