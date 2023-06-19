#include <bluefruit.h>
#include "jobsMints.h"
#include "devicesMints.h"
#include <Arduino.h>

// #include <Adafruit_LittleFS.h>
// #include <InternalFileSystem.h>
// #include <Wire.h>
// #include <Adafruit_NeoPixel.h>
// #include "Adafruit_MCP9808.h"
uint8_t sensorDataBuffer[3];

Adafruit_MCP9808 mcp9808 = Adafruit_MCP9808();
bool MCP9808Online; 

// // BLE transmit buffer

bool BLEOnline; 


// #define TEMPERATURE_F
// // uncomment the following line if you want to read temperature in degrees Celsius
// #define TEMPERATURE_C

// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery

// constants won't change. Used here to set a pin number:
const int ledPin = 3;  // the number of the LED pin

// Variables will change:
int ledState = LOW;  // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated
int PulseSensorPurplePin = 0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0

int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;    

unsigned long blinkStartTime = millis();
unsigned long blinkPeriod = 10000;

// constants won't change:
const long interval = 1000;  // interval at which to blink (milliseconds)

uint16_t sensingPeriod = 2000;
uint16_t initPeriod = 1500;

unsigned long startTime;


sensorPacket liveDataPacket;

void setup() {

  delay(initPeriod);
  initializeSerialMints();

  Serial.println("MINTS Wearable arm band v1.0");
  Serial.println("-------------------------------------\n");

  delay(initPeriod);
  MCP9808Online = initializeMCP9808Mints();

  BLEOnline = initializeBLEMints();

}


void loop() {
    startTime  = millis();
      
      delay(sensingPeriod);
      if(MCP9808Online)
      {
        readMCP9808Mints();
      }
 
       delay(sensingPeriod);
      if(MCP9808Online)
      {
        readMCP9808MintsTest();
      }
  // Signal = analogRead(A0);  // Read the PulseSensor's value. 
  // Serial.println(Signal);  
  // snprintf(temperature_buf, sizeof(temperature_buf) - 1, "%0.*f", 1, Signal*1.0);
  // bleuart.write(temperature_buf);

  delayMints(millis() - startTime,2000);
  
  }
