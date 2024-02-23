# TCP Image-Publisher Client for ESP32-CAM

This is a TCP image-publisher client designed for the ESP32-CAM microcontroller. It allows you to capture images using the ESP32-CAM and publish them over a TCP connection.

## Features

- Image capture: The client can capture images using the ESP32-CAM's camera module.
- TCP connection: It establishes a TCP connection with a server to publish the captured images.
- Configurable settings: You can customize various settings such as image resolution, quality, and server address.

## Prerequisites

Before using this client, make sure you have the following:

- ESP32-CAM microcontroller: This client is specifically designed for the ESP32-CAM board.
- Server: You need a server that can receive and process the images published by the client.
- Development environment: Set up the necessary development environment for programming the ESP32-CAM.

## Installation

1. Clone or download this repository.
2. Open the project in your preferred development environment.
3. Configure the necessary settings in the `config.h` file.
4. Build and upload the code to your ESP32-CAM.

## Usage

1. Power on the ESP32-CAM.
2. The client will automatically connect to the configured server.
3. The ESP32-CAM will start capturing images and publishing them over the TCP connection.
4. Monitor the server to receive and process the images.

## Configuration

You can customize the following settings in the `config.h` file:

- `WIFI_SSID`: The SSID of the Wi-Fi network to connect to.
- `WIFI_PASS`: The password of the Wi-Fi network.
- `SERVER_IP`: The IP address or hostname of the server.
- `SERVER_PORT`: The port number of the server.
- `CAM_FRAME_SIZE`: The resolution of the captured images.
- `CAM_JPEG_QUALITY`: The quality of the captured images.

and more...

