#ifndef defs_h
#define defs_h

#include <Arduino.h>

#include <Sensors.h>

SamIoT::Sensors::DHT SouthBedDHT(12);
SamIoT::Sensors::DHT NorthBedDHT(13);
SamIoT::Sensors::HCSR04 SouthBedUltrasonic(5, 4);
SamIoT::Sensors::HCSR04 NorthBedUltrasonic(14, 12);
#endif