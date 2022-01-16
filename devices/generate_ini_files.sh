#!/bin/bash

source ./env.sh

devices=$(pwd)
solar_controller="$devices/solar-controller"

echo "Writing solar controller ini file."
cat > "$solar_controller/platformio.ini" <<- EOF
; PlatformIO Project Configuration File
;
;   Build options: build flags, source filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:lolin_d32_pro]
platform = espressif32
board = lolin_d32_pro
build_type = debug
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp32_exception_decoder
monitor_flags = --raw
board_build.mcu = esp32
board_build.f_cpu = 240000000L
upload_protocol = espota
upload_port=$SOLAR_CONTROLLER_HOSTNAME.local
upload_flags =
    --auth=$SOLAR_CONTROLLER_OTA_PASS
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    jandrassy/TelnetStream @ ^1.2.1
EOF