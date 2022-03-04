#ifndef defs_h
#define defs_h

#include <Arduino.h>

#include <Sensors.h>

SamIoT::Sensors::DHT DHT(13);
SamIoT::Sensors::HCSR04 Ultrasonic(5, 4);
#endif