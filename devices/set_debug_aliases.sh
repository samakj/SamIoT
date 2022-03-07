#!/bin/bash

source ./env.sh

alias debug_mesh_bridge="telnet $DEVICE_IP_MASK$MESH_BRIDGE_IP_LOCATION"
alias debug_solar_controller="telnet $DEVICE_IP_MASK$SOLAR_CONTROLLER_IP_LOCATION"
alias debug_water_butt="telnet $DEVICE_IP_MASK$WATER_BUTT_IP_LOCATION"
alias debug_pond="telnet $DEVICE_IP_MASK$POND_IP_LOCATION"
alias debug_aquaponics="telnet $DEVICE_IP_MASK$AQUAPONICS_IP_LOCATION"