#include <stdio.h>
#include "wifi_manager.h"
#include "camera.h"
#include "image_publisher.h"
#include "pthread.h"

// thread function to publish frames
pthread_t frame_publisher_thread;
pthread_t accept_new_clients_thread;
void app_main(void)
{
    
    connect_to_wifi("da lab", "helloabc");

    pthread_create(&frame_publisher_thread, NULL,capture_frame , NULL);
    

}