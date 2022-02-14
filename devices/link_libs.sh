#!/bin/bash

devices=$(pwd)
root="$(dirname $devices)"
shared="$root/shared"
cpp="$shared/c++"
solar_controller="$devices/solar-controller"

AsyncDHT="$cpp/AsyncDHT"      
AsyncNTP="$cpp/AsyncNTP"      
AsyncOTA="$cpp/AsyncOTA"      
AsyncPZEM="$cpp/AsyncPZEM"     
AsyncSwitch="$cpp/AsyncSwitch"  
AsyncTracer="$cpp/AsyncTracer"   
AsyncWifi="$cpp/AsyncWifi"     
DeviceServer="$cpp/DeviceServer"  
ExecutionTimer="$cpp/ExecutionTimer"
Log="$cpp/Log"
TimeUtils="$cpp/TimeUtils"

ln -s "$AsyncDHT" "$solar_controller/lib"
ln -s "$AsyncNTP" "$solar_controller/lib"
ln -s "$AsyncOTA" "$solar_controller/lib"
ln -s "$AsyncPZEM" "$solar_controller/lib"
ln -s "$AsyncSwitch" "$solar_controller/lib"
ln -s "$AsyncTracer" "$solar_controller/lib"
ln -s "$AsyncWifi" "$solar_controller/lib"
ln -s "$DeviceServer" "$solar_controller/lib"
ln -s "$ExecutionTimer" "$solar_controller/lib"
ln -s "$Log" "$solar_controller/lib"
ln -s "$TimeUtils" "$solar_controller/lib"

ln -s "$shared" "$solar_controller/shared"
