#ifndef DEVICES_MINTS_H
#define DEVICES_MINTS_H
//
#include <Arduino.h>
#include "jobsMints.h"
#include <bluefruit.h>
#include "Adafruit_MCP9808.h"
#include <iostream>
#include <bitset>


// mcp9808
#define IIC_ADDR  uint8_t(0x77)

//
extern Adafruit_MCP9808 mcp9808;
// #define MCP9808_REG_AMBIENT_TEMP 0x05 ;
bool initializeMCP9808Mints();
void readMCP9808Mints();
void readMCP9808MintsTest();


extern uint16_t PSA1093Signal;
bool initializePSA1093Mints();
void readPSA1093Mints();


//

// // BLE Service
extern BLEDfu  bledfu;  // OTA DFU service
extern BLEDis  bledis;  // device information
extern BLEUart bleuart; // uart over ble
extern BLEBas  blebas;  // battery

bool initializeBLEMints();
void startAdv(void);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void connect_callback(uint16_t conn_handle);

extern uint8_t sensorDataBuffer[4];

struct sensorPacket
{
    uint8_t sensorID;
    uint8_t parametorID;
    uint16_t sensorData;
    };

extern sensorPacket liveDataPacket;




#endif
