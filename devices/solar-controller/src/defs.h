#ifndef defs_h
#define defs_h

#include <Arduino.h>

#include <AsyncPZEM.h>
#include <AsyncOutput.h>
#include <AsyncTracer.h>
#include <DeviceServer.h>

DeviceState State;
uint8_t TRACER_TX = 22;
uint8_t TRACER_RX = 23;
HardwareSerial TracerSerial(1);
AsyncTracer Tracer(&TracerSerial, TRACER_TX, TRACER_RX, 0x01);
AsyncOutput Relay1(21, HIGH);
AsyncOutput Relay2(19, HIGH);
AsyncOutput Relay3(18, HIGH);
AsyncOutput Relay4(5, HIGH);
AsyncOutput Relay5(4, HIGH);
AsyncOutput Relay6(0, HIGH);
AsyncOutput Relay7(2, HIGH);
AsyncOutput Relay8(15, HIGH);
AsyncOutput InverterRelay(13, LOW);

#endif