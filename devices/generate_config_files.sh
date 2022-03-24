#!/bin/bash

. ./env.sh

devices=$(pwd)
mesh_bridge="$devices/mesh-bridge"
solar_controller="$devices/solar-controller"
water_butt="$devices/water-butt"
pond="$devices/pond"
aquaponics="$devices/aquaponics"

echo "Writing mesh bridge config file."
cat > "$mesh_bridge/src/config.h" <<- EOF
#ifndef config_h
#define config_h

#include <Mesh.h>

const char HOSTNAME[24] = "$MESH_BRIDGE_HOSTNAME";
const char LOCATION[24] = "$MESH_BRIDGE_LOCATION";
uint8_t IP_LOCATION = $MESH_BRIDGE_IP_LOCATION;
const char OTA_PASSWORD[24] = "$MESH_BRIDGE_OTA_PASS";

SamIoT::Mesh::WifiCredentials Patty = {"$PATTY_SSID", "$PATTY_PASS"};
SamIoT::Mesh::WifiCredentials Selma = {"$SELMA_SSID", "$SELMA_PASS"};
SamIoT::Mesh::WifiCredentials TheVale = {"$VALE_SSID", "$VALE_PASS"};

SamIoT::Mesh::MeshCredentials Marge = {"$MESH_SSID", "$MESH_PASSWORD"};

#endif
EOF

echo "Writing solar controller config file."
cat > "$solar_controller/src/config.h" <<- EOF
#ifndef config_h
#define config_h

#include <AsyncWifi.h>

const char HOSTNAME[24] = "$SOLAR_CONTROLLER_HOSTNAME";
const char LOCATION[24] = "$SOLAR_CONTROLLER_LOCATION";
uint8_t IP_LOCATION = $SOLAR_CONTROLLER_IP_LOCATION;
const char OTA_PASSWORD[24] = "$SOLAR_CONTROLLER_OTA_PASS";

WifiCredentials Patty("$PATTY_SSID", "$PATTY_PASS");
WifiCredentials Selma("$SELMA_SSID", "$SELMA_PASS");
WifiCredentials TheVale("$VALE_SSID", "$VALE_PASS");

MeshCredentials Marge("$MESH_SSID", "$MESH_PASSWORD");

#endif
EOF

echo "Writing water butt config file."
cat > "$water_butt/src/config.h" <<- EOF
#ifndef config_h
#define config_h

#include <Mesh.h>

const char HOSTNAME[24] = "$WATER_BUTT_HOSTNAME";
const char LOCATION[24] = "$WATER_BUTT_LOCATION";
uint8_t IP_LOCATION = $WATER_BUTT_IP_LOCATION;
const char OTA_PASSWORD[24] = "$WATER_BUTT_OTA_PASS";

SamIoT::Mesh::WifiCredentials Patty = {"$PATTY_SSID", "$PATTY_PASS"};
SamIoT::Mesh::WifiCredentials Selma = {"$SELMA_SSID", "$SELMA_PASS"};
SamIoT::Mesh::WifiCredentials TheVale = {"$VALE_SSID", "$VALE_PASS"};

SamIoT::Mesh::MeshCredentials Marge = {"$MESH_SSID", "$MESH_PASSWORD"};

#endif
EOF

echo "Writing pond config file."
cat > "$pond/src/config.h" <<- EOF
#ifndef config_h
#define config_h

#include <Mesh.h>

const char HOSTNAME[24] = "$POND_HOSTNAME";
const char LOCATION[24] = "$POND_LOCATION";
uint8_t IP_LOCATION = $POND_IP_LOCATION;
const char OTA_PASSWORD[24] = "$POND_OTA_PASS";

SamIoT::Mesh::WifiCredentials Patty = {"$PATTY_SSID", "$PATTY_PASS"};
SamIoT::Mesh::WifiCredentials Selma = {"$SELMA_SSID", "$SELMA_PASS"};
SamIoT::Mesh::WifiCredentials TheVale = {"$VALE_SSID", "$VALE_PASS"};

SamIoT::Mesh::MeshCredentials Marge = {"$MESH_SSID", "$MESH_PASSWORD"};

#endif
EOF

echo "Writing aquqaponics config file."
cat > "$aquaponics/src/config.h" <<- EOF
#ifndef config_h
#define config_h

#include <Mesh.h>

const char HOSTNAME[24] = "$AQUAPONICS_HOSTNAME";
const char LOCATION[24] = "$AQUAPONICS_LOCATION";
uint8_t IP_LOCATION = $AQUAPONICS_IP_LOCATION;
const char OTA_PASSWORD[24] = "$AQUAPONICS_OTA_PASS";

SamIoT::Mesh::WifiCredentials Patty = {"$PATTY_SSID", "$PATTY_PASS"};
SamIoT::Mesh::WifiCredentials Selma = {"$SELMA_SSID", "$SELMA_PASS"};
SamIoT::Mesh::WifiCredentials TheVale = {"$VALE_SSID", "$VALE_PASS"};

SamIoT::Mesh::MeshCredentials Marge = {"$MESH_SSID", "$MESH_PASSWORD"};

#endif
EOF