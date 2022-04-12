#!/bin/bash

. ./env.sh

devices=$(pwd)
mesh_bridge="$devices/mesh-bridge"
solar_controller="$devices/solar-controller"
water_butt="$devices/water-butt"
pond="$devices/pond"
aquaponics="$devices/aquaponics"
chicken_coop="$devices/chicken-coop"

echo "Writing mesh bridge ini file."
cat > "$mesh_bridge/platformio.ini" <<- EOF
; PlatformIO Project Configuration File
;
;   Build options: build flags, . filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:d1]
platform = espressif8266
board = d1
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp8266_exception_decoder
monitor_flags = --raw
board_build.mcu = esp8266
board_build.f_cpu = 80000000L
upload_protocol = espota
upload_port=$MESH_BRIDGE_HOSTNAME.local
upload_flags =
    --auth=$MESH_BRIDGE_OTA_PASS
lib_ldf_mode = deep+
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    me-no-dev/ESP Async WebServer @ ^1.2.3
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESPAsyncTCP @ ^1.2.2
    painlessmesh/painlessMesh @ ^1.4.10

[env:lolin_d32_pro]
platform = https://github.com/jason2866/platform-espressif32.git
board = lolin_d32_pro
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp32_exception_decoder
monitor_flags = --raw
board_build.mcu = esp32
board_build.f_cpu = 240000000L
upload_protocol = espota
upload_port=$MESH_BRIDGE_HOSTNAME.local
upload_flags =
    --auth=$MESH_BRIDGE_OTA_PASS
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    painlessmesh/painlessMesh @ ^1.4.10
EOF

echo "Writing solar controller ini file."
cat > "$solar_controller/platformio.ini" <<- EOF
; PlatformIO Project Configuration File
;
;   Build options: build flags, . filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:lolin_d32_pro]
platform = https://github.com/jason2866/platform-espressif32.git
board = lolin_d32_pro
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
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
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    painlessmesh/painlessMesh @ ^1.4.10
EOF

echo "Writing water butt ini file."
cat > "$water_butt/platformio.ini" <<- EOF
; PlatformIO Project Configuration File
;
;   Build options: build flags, . filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:d1]
platform = espressif8266
board = d1
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp8266_exception_decoder
monitor_flags = --raw
board_build.mcu = esp8266
board_build.f_cpu = 80000000L
upload_protocol = espota
upload_port=$WATER_BUTT_HOSTNAME.local
upload_flags =
    --auth=$WATER_BUTT_OTA_PASS
lib_ldf_mode = deep+
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    me-no-dev/ESP Async WebServer @ ^1.2.3
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESPAsyncTCP @ ^1.2.2
    painlessmesh/painlessMesh @ ^1.4.10

[env:lolin_d32_pro]
platform = https://github.com/jason2866/platform-espressif32.git
board = lolin_d32_pro
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp32_exception_decoder
monitor_flags = --raw
board_build.mcu = esp32
board_build.f_cpu = 240000000L
upload_protocol = espota
upload_port=$WATER_BUTT_HOSTNAME.local
upload_flags =
    --auth=$WATER_BUTT_OTA_PASS
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    painlessmesh/painlessMesh @ ^1.4.10
EOF

echo "Writing pond ini file."
cat > "$pond/platformio.ini" <<- EOF
; PlatformIO Project Configuration File
;
;   Build options: build flags, . filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:d1]
platform = espressif8266
board = d1
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp8266_exception_decoder
monitor_flags = --raw
board_build.mcu = esp8266
board_build.f_cpu = 80000000L
upload_protocol = espota
upload_port=$POND_HOSTNAME.local
upload_flags =
    --auth=$POND_OTA_PASS
lib_ldf_mode = deep+
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    me-no-dev/ESPAsyncTCP @ ^1.2.2
    painlessmesh/painlessMesh @ ^1.4.10

[env:lolin_d32_pro]
platform = https://github.com/jason2866/platform-espressif32.git
board = lolin_d32_pro
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp32_exception_decoder
monitor_flags = --raw
board_build.mcu = esp32
board_build.f_cpu = 240000000L
upload_protocol = espota
upload_port=$POND_HOSTNAME.local
upload_flags =
    --auth=$POND_OTA_PASS
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    painlessmesh/painlessMesh @ ^1.4.10
EOF

echo "Writing aquaponics ini file."
cat > "$aquaponics/platformio.ini" <<- EOF
; PlatformIO Project Configuration File
;
;   Build options: build flags, . filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:d1]
platform = espressif8266
board = d1
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp8266_exception_decoder
monitor_flags = --raw
board_build.mcu = esp8266
board_build.f_cpu = 80000000L
upload_protocol = espota
upload_port=$AQUAPONICS_HOSTNAME.local
upload_flags =
    --auth=$AQUAPONICS_OTA_PASS
lib_ldf_mode = deep+
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    me-no-dev/ESPAsyncTCP @ ^1.2.2
    painlessmesh/painlessMesh @ ^1.4.10

[env:lolin_d32_pro]
platform = https://github.com/jason2866/platform-espressif32.git
board = lolin_d32_pro
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp32_exception_decoder
monitor_flags = --raw
board_build.mcu = esp32
board_build.f_cpu = 240000000L
upload_protocol = espota
upload_port=$AQUAPONICS_HOSTNAME.local
upload_flags =
    --auth=$AQUAPONICS_OTA_PASS
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    painlessmesh/painlessMesh @ ^1.4.10
EOF

echo "Writing chicken coop ini file."
cat > "$chicken_coop/platformio.ini" <<- EOF
; PlatformIO Project Configuration File
;
;   Build options: build flags, . filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:d1]
platform = espressif8266
board = d1
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp8266_exception_decoder
monitor_flags = --raw
board_build.mcu = esp8266
board_build.f_cpu = 80000000L
upload_protocol = espota
upload_port=$CHICKEN_COOP_HOSTNAME.local
upload_flags =
    --auth=$CHICKEN_COOP_OTA_PASS
lib_ldf_mode = deep+
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    me-no-dev/ESPAsyncTCP @ ^1.2.2
    painlessmesh/painlessMesh @ ^1.4.10

[env:lolin_d32_pro]
platform = https://github.com/jason2866/platform-espressif32.git
board = lolin_d32_pro
build_type = debug
build_unflags = 
    -std=gnu++11
build_flags = 
    -std=gnu++17
framework = arduino
monitor_speed = 115200
monitor_filters = colorize, esp32_exception_decoder
monitor_flags = --raw
board_build.mcu = esp32
board_build.f_cpu = 240000000L
upload_protocol = espota
upload_port=$CHICKEN_COOP_HOSTNAME.local
upload_flags =
    --auth=$CHICKEN_COOP_OTA_PASS
lib_deps = 
    4-20ma/ModbusMaster @ ^2.0.1
    adafruit/Adafruit Unified Sensor @ ^1.1.4
    adafruit/DHT sensor library @ ^1.4.2
    bblanchon/ArduinoJson@ ^6.17.3
    jandrassy/TelnetStream @ ^1.2.1
    mandulaj/PZEM-004T-v30 @ ^1.1.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    painlessmesh/painlessMesh @ ^1.4.10
EOF