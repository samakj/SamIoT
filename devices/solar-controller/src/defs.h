#ifndef defs_h
#define defs_h

#include <Arduino.h>

#include <AsyncPZEM.h>
#include <AsyncOutput.h>
#include <AsyncTracer.h>
#include <DeviceServer.h>

DeviceState State;
uint8_t PZEM_SERIAL_1_TX = 22;
uint8_t PZEM_SERIAL_1_RX = 23;
uint8_t PZEM_SERIAL_2_TX = 14;
uint8_t PZEM_SERIAL_2_RX = 27;
HardwareSerial PZEMSerial1(1);
HardwareSerial PZEMSerial2(2);
// AsyncTracer Tracer(&ModbusSerial, MODBUS_TX, MODBUS_RX, 0x01);
AsyncPZEM PZEM1(&PZEMSerial1, PZEM_SERIAL_1_RX, PZEM_SERIAL_1_TX, 0x11);
AsyncPZEM PZEM2(&PZEMSerial1, PZEM_SERIAL_1_RX, PZEM_SERIAL_1_TX, 0x12);
AsyncPZEM PZEM3(&PZEMSerial2, PZEM_SERIAL_2_RX, PZEM_SERIAL_2_TX, 0x13);
AsyncPZEM PZEM4(&PZEMSerial2, PZEM_SERIAL_2_RX, PZEM_SERIAL_2_TX, 0x14);
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