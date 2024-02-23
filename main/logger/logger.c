#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/apps/sntp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


#define SERVER_IP "localhost"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define SNTP_SERVER_NAME "pool.ntp.org"
#define WAIT_FOR_SNTP_SYNC_TIME_MS 5000
#define MAX_CLIENTS 5  // The maximum number of clients you want to handle simultaneously.

int client_sockets[MAX_CLIENTS] = {0};  // Array to keep track of client sockets.


int sock;  // Global socket descriptor
struct timespec ts;

void initialize_sntp(void) {
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, SNTP_SERVER_NAME);
    sntp_init();
}

int initialize_time(void) {

    // Initialize SNTP
    initialize_sntp();

     // Wait for time synchronization
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) { // 2016 is just an example, use any year post Unix epoch
        vTaskDelay(WAIT_FOR_SNTP_SYNC_TIME_MS / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
    // Disconnect from the SNTP server and WiFi
    // sntp_stop();

    if (retry == retry_count) {
        return -1; // Failed to sync time
    } else {
        return 0; // Time synchronized
    }
}

char* format_log_message(char* controller_name, char* message, char* message_type){
    char* formatted_message = malloc(BUFFER_SIZE);
    
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        perror("clock_gettime");
        free(formatted_message); // Free the memory on failure.
        return NULL;
    }
    
    sprintf(formatted_message, "%lld.%09ld::%s::%s::%s\t", ts.tv_sec, ts.tv_nsec,message_type, controller_name, message);
    printf("%s\n", formatted_message);
    return formatted_message;
}



int initialize_socket() {
    struct sockaddr_in server_address;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket created\n");

    // Set up the server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces.

    // Bind to the specified port and IP
    if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Binding failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(sock, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        return -1;
    }
    return 0;
}

void log_message(char* controller_name, char* message, char* message_type) {

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0) {  // Check if the client socket is valid.
            send(client_sockets[i], format_log_message(controller_name, message,message_type), BUFFER_SIZE, 0);
        }
    }
    
}

// Call this function in a loop or task to accept clients and store their sockets.
void accept_clients() {
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);
    int new_socket;
    initialize_socket();
    log_message("ControllerManager", "Socket initialized","INFO");
    
    initialize_time();
    log_message("ControllerManager", "Time initialized","INFO");
    while (1)
    {
        new_socket = accept(sock, (struct sockaddr *)&client_address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        printf("Accepted new client\n");

        // Add new socket to the array of client sockets
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = new_socket;
                break;
            }
        }
    }
    
    
}