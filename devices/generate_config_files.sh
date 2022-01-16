#!/bin/bash

source ./env.sh

devices=$(pwd)
solar_controller="$devices/solar-controller"

echo "Writing solar controller config file."
cat > "$solar_controller/src/config.h" <<- EOF
#ifndef config_h
#define config_h

#include <AsyncWifi.h>

const char HOSTNAME[24] = "$SOLAR_CONTROLLER_HOSTNAME";
const char LOCATION[24] = "$SOLAR_CONTROLLER_LOCATION";
int IP_LOCATION = $SOLAR_CONTROLLER_IP_LOCATION;
const char OTA_PASSWORD[24] = "$SOLAR_CONTROLLER_OTA_PASS";

WifiCredentials Patty("$PATTY_SSID", "$PATTY_PASS");
WifiCredentials Selma("$SELMA_SSID", "$SELMA_PASS");
WifiCredentials TheVale("$VALE_SSID", "$VALE_PASS");

#endif
EOF