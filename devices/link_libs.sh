#!/bin/bash

devices=$(pwd)
root="$(dirname $devices)"
shared="$root/shared"
cpp="$shared/c++"
solar_controller="$devices/solar-controller"
water_butt="$devices/water-butt"
pond="$devices/pond"
aquaponics="$devices/aquaponics"

AsyncDHT="$cpp/AsyncDHT"      
AsyncMesh="$cpp/AsyncMesh"      
AsyncNTP="$cpp/AsyncNTP"      
AsyncOTA="$cpp/AsyncOTA"      
AsyncPZEM="$cpp/AsyncPZEM"     
AsyncSwitch="$cpp/AsyncSwitch"  
AsyncTracer="$cpp/AsyncTracer"   
AsyncWifi="$cpp/AsyncWifi"     
DeviceServer="$cpp/DeviceServer"  
ExecutionTimer="$cpp/ExecutionTimer"
Log="$cpp/Log"
MeasurementsState="$cpp/MeasurementsState"
TimeUtils="$cpp/TimeUtils"

rm -rf "$solar_controller/lib"
ln -s "$cpp" "$solar_controller/lib"

rm -rf "$solar_controller/shared"
ln -s "$shared" "$solar_controller/shared"

rm -rf "$water_butt/lib"
ln -s "$cpp" "$water_butt/lib"

rm -rf "$water_butt/shared"
ln -s "$shared" "$water_butt/shared"

rm -rf "$pond/lib"
ln -s "$cpp" "$pond/lib"

rm -rf "$pond/shared"
ln -s "$shared" "$pond/shared"

rm -rf "$aquaponics/lib"
ln -s "$cpp" "$aquaponics/lib"

rm -rf "$aquaponics/shared"
ln -s "$shared" "$aquaponics/shared"
