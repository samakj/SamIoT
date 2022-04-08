#ifndef defs_h
#define defs_h

#include <Arduino.h>

#include <Sensors.h>

SamIoT::Sensors::DHT CoopDHT(2);
SamIoT::Sensors::DHT ShadeDHT(4);
SamIoT::Sensors::DHT RunDHT(5);
#endif