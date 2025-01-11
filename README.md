[https://github.com/nochkin/esp_test.git](https://github.com/nochkin/esp_test.git)

# ESP Test

Welcome to the **ESP Test** project.
This repository provides a simple code for various Espressif microcontrollers and serves a purpose to test basic functionality.
It uses PlatformIO framework to download all required dependencies and build firmware files.

## Supported microcontrollers/boards

- ESP32-S3
  * N16R8 and N8R2 variation [boards](https://github.com/vcc-gnd/YD-ESP32-S3)
  * WS2812 LED on GPIO 48
- ESP32-S3 Super Mini
  * WS2812 LED on GPIO 48
- ESP32-C6 Super Mini
  * WS2812 LED on GPIO 8
- ESP32-C3 Super Mini
  * LED on GPIO 8
- Lilygo T-Display (ESP32)
  * ST7789 SPI LCD
- ESP32
- ESP8266

## Prerequisites

Before you get started, ensure you have the following installed:

- [PlatformIO](https://platformio.org/)
- A compatible ESP board listed above
- USB cable for flashing

## Getting Started

Follow these steps to set up and run the project:

### 1. Clone the Repository

```bash
git clone https://github.com/nochkin/esp_test.git
cd esp_test
```
### 2. Build

```bash
pio run -e "board" -t upload
```
Where "board" is any of the following:
- esp32s3_n16r8
- esp32s3_n8r2
- esp32s3_supermini
- esp32c6_supermini
- esp32c3_supermini
- esp32_tdisplay
- esp32
- esp8266

The command will automatically download all required libraries, compilers, then it will build the firmware and upload it to the connected board.

### 3. Monitor Serial Output

Check the Serial port as the most diagnostics will be sent there.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---
