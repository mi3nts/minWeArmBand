#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_MCP9808.h"
#include <Arduino.h>

#define TEMPERATURE_F
// uncomment the following line if you want to read temperature in degrees Celsius
//#define TEMPERATURE_C

// Feather NRF52840 Built-in NeoPixel
#define PIN 16
Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);

// BLE transmit buffer
char temperature_buf [3];

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

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



// void startAdv(void)
// {

// // Advertising Packet
//   // Advertising packet
//   Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
//   Bluefruit.Advertising.addTxPower();

//   // Include bleuart 128-bit uuid
//   Bluefruit.Advertising.addService(bleuart);

//   // Secondary Scan Response packet (optional)
//   // Since there is no room for 'Name' in Advertising packet
//   Bluefruit.ScanResponse.addName();
  
//   /* Start Advertising
//    * - Enable auto advertising if disconnected
//    * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
//    * - Timeout for fast mode is 30 seconds
//    * - Start(timeout) with timeout = 0 will advertise forever (until connected)
//    * 
//    * For recommended advertising interval
//    * https://developer.apple.com/library/content/qa/qa1931/_index.html   
//    */
//   Bluefruit.Advertising.restartOnDisconnect(true);
//   Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
//   Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
//   Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
// }

// // callback invoked when central connects
// void connect_callback(uint16_t conn_handle)
// {
//   // Get the reference to current connection
//   BLEConnection* connection = Bluefruit.Connection(conn_handle);

//   char central_name[32] = { 0 };
//   connection->getPeerName(central_name, sizeof(central_name));

//   Serial.print("Connected to ");
//   Serial.println(central_name);
// }

// /**
//  * Callback invoked when a connection is dropped
//  * @param conn_handle connection where this event happens
//  * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
//  */
// void disconnect_callback(uint16_t conn_handle, uint8_t reason)
// {
//   (void) conn_handle;
//   (void) reason;

//   Serial.println();
//   Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
// }

void setup() {

  Serial.begin(115200);
  delay(1000); 
  Serial.println("Wearable BlueFruit Temperature Sensor");
  Serial.println("-------------------------------------\n");


  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
  Serial.println("Found MCP9808!");

  // Sets the resolution of reading
  tempsensor.setResolution(3);

  // Configure BLE
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth 
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("MINTS");
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("MINTS");
  bledis.setModel("Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

  // Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");

  // // initialize neopixel object
  // pixels.begin();

  // // set all pixel colors to 'off'
  // pixels.clear();
}


void loop() {

  Signal = analogRead(A0);  // Read the PulseSensor's value. 
  Serial.println(Signal);  
  snprintf(temperature_buf, sizeof(temperature_buf) - 1, "%0.*f", 1, Signal*1.0);
  bleuart.write(temperature_buf);
  // delay(10);
  }
  //   // if the LED is off turn it on and vice-versa:
  //   if (Signal>Threshold) {
  //     ledState = HIGH;
  //   } else {
  //     ledState = LOW;
  //   }
  //   digitalWrite(ledPin, ledState);
  //   // set the LED with the ledState of the variable:



  // if (millis() - blinkStartTime >= blinkPeriod)  //LED blink time check
  // {
  //   Serial.println(Signal); 
    
  //   tempsensor.wake();
  //   float c = tempsensor.readTempC();
  //   Serial.print(c, 4); Serial.println(" C\t"); 
  //   tempsensor.shutdown_wake(1); 
  //   blinkStartTime = blinkStartTime + blinkPeriod;  //save the new blink start time
  //   snprintf(temperature_buf, sizeof(temperature_buf) - 1, "%0.*f", 1, c);
  //   bleuart.write(temperature_buf);
  // }


    
  // }


// // /**************************************************************************/
// // /*!
// // This is a demo for the Adafruit MCP9808 breakout
// // ----> http://www.adafruit.com/products/1782
// // Adafruit invests time and resources providing this open source code,
// // please support Adafruit and open-source hardware by purchasing
// // products from Adafruit!
// // */
// // /**************************************************************************/

// // #include <Wire.h>
// // #include <Arduino.h>
// // #include "Adafruit_MCP9808.h"

// // //  Variables
// // int PulseSensorPurplePin = 0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
// // int LED3 = 3;   //  The on-board Arduion LED


// // int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
// // int Threshold = 50;            // Determine which Signal to "count as a beat", and which to ingore. 


// // // The SetUp Function:
// // void setup() {
// //   pinMode(LED3,OUTPUT);   
// //   pinMode(A0,INPUT);        // pin that will blink to your heartbeat!
// //    Serial.begin(9600);         // Set's up Serial Communication at certain speed. 

// //    Serial.println("Serial On")  ;

// // }

// // // The Main Loop Function
// // void loop() {

// //   Signal = analogRead(A0);  // Read the PulseSensor's value. 
// //     Serial.println(Signal);                                           // Assign this value to the "Signal" variable.
// //   // if (Signal != 0){
// //   //  Serial.println(Signal);  
// //   // //  delay(10000);                  // Send the Signal value to Serial Plotter.
// //   // }
   
// //    if(Signal > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.  
// //      digitalWrite(LED3,HIGH);        
// //    } else {
// //      digitalWrite(LED3,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
// //    }


// // delay(10);
   
   
// // }

// #include "Adafruit_MCP9808.h"
// #include <Arduino.h>


// Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
// // constants won't change. Used here to set a pin number:
// const int ledPin = 3;  // the number of the LED pin

// // Variables will change:
// int ledState = LOW;  // ledState used to set the LED

// // Generally, you should use "unsigned long" for variables that hold time
// // The value will quickly become too large for an int to store
// unsigned long previousMillis = 0;  // will store last time LED was updated
// int PulseSensorPurplePin = 0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0

// int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
// int Threshold = 550;    

// unsigned long blinkStartTime = millis();
// unsigned long blinkPeriod = 10000;

// // constants won't change:
// const long interval = 1000;  // interval at which to blink (milliseconds)

// void setup() {
//   // set the digital pin as output:
//   pinMode(ledPin, OUTPUT);
//   pinMode(A0,INPUT);        // pin that will blink to your heartbeat!
//   Serial.begin(9600);         // Set's up Serial Communication at certain speed. 
//   Serial.println("Serial On")  ;

//   if (!tempsensor.begin(0x18)) {
//     Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
//     while (1);
//   }
    
//    Serial.println("Found MCP9808!");

//   tempsensor.setResolution(3);

// }

// void loop() {

//   Signal = analogRead(A0);  // Read the PulseSensor's value. 
//   // Serial.println(Signal);  
  
//     // if the LED is off turn it on and vice-versa:
//     if (Signal>Threshold) {
//       ledState = HIGH;
//     } else {
//       ledState = LOW;
//     }
//     digitalWrite(ledPin, ledState);
//     // set the LED with the ledState of the variable:

//   if (millis() - blinkStartTime >= blinkPeriod)  //LED blink time check
//   {
//     Serial.println(Signal); 
//     tempsensor.wake();
//     float c = tempsensor.readTempC();
//     Serial.print(c, 4); Serial.println(" C\t"); 
//     tempsensor.shutdown_wake(1); 
//     blinkStartTime = blinkStartTime + blinkPeriod;  //save the new blink start time
//   }


    
//   }



#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_MCP9808.h"
#include <Arduino.h>

#define TEMPERATURE_F
// uncomment the following line if you want to read temperature in degrees Celsius
//#define TEMPERATURE_C

// Feather NRF52840 Built-in NeoPixel
#define PIN 16
Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);

// BLE transmit buffer
char temperature_buf [8];

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

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



void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}

void setup() {

  Serial.begin(115200);
  delay(20000); 
  Serial.println("Wearable BlueFruit Temperature Sensor");
  Serial.println("-------------------------------------\n");


  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
  Serial.println("Found MCP9808!");

  // Sets the resolution of reading
  tempsensor.setResolution(3);

  // Configure BLE
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth 
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("Bluefruit52");
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");

  // initialize neopixel object
  pixels.begin();

  // set all pixel colors to 'off'
  pixels.clear();
}


void loop() {

  Signal = analogRead(A0);  // Read the PulseSensor's value. 
  // Serial.println(Signal);  
  
    // if the LED is off turn it on and vice-versa:
    if (Signal>Threshold) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
    // set the LED with the ledState of the variable:
    snprintf(temperature_buf, sizeof(temperature_buf) - 1, "%0.*f", 1, Signal*1.0);
    bleuart.write(temperature_buf);
    delay(10);

  if (millis() - blinkStartTime >= blinkPeriod)  //LED blink time check
  {
    Serial.println(Signal); 
    
    tempsensor.wake();
    float c = tempsensor.readTempC();
    Serial.print(c, 4); Serial.println(" C\t"); 
    tempsensor.shutdown_wake(1); 
    blinkStartTime = blinkStartTime + blinkPeriod;  //save the new blink start time
    // snprintf(temperature_buf, sizeof(temperature_buf) - 1, "%0.*f", 1, Signal*1.0);
    // bleuart.write(temperature_buf);
  }
    
}






// // SPDX-FileCopyrightText: 2020 Brent Rubell for Adafruit Industries
// //
// // SPDX-License-Identifier: MIT

// /*********************************************************************
//  Learn Guide: BLE Temperature Monitoring Armband

//  Adafruit invests time and resources providing this open source code,
//  please support Adafruit and open-source hardware by purchasing
//  products from Adafruit!

//  MIT license, check LICENSE for more information
//  All text above, and the splash screen below must be included in
//  any redistribution
// *********************************************************************/
// #include <bluefruit.h>
// #include <Adafruit_LittleFS.h>
// #include <InternalFileSystem.h>
// #include <Wire.h>
// #include <Adafruit_NeoPixel.h>
// #include "Adafruit_MCP9808.h"

// // Read temperature in degrees Fahrenheit
// #define TEMPERATURE_F
// // uncomment the following line if you want to read temperature in degrees Celsius
// //#define TEMPERATURE_C

// // Feather NRF52840 Built-in NeoPixel
// #define PIN 16
// Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);

// // Maximum temperature value for armband's fever indicator
// // NOTE: This is in degrees Fahrenheit
// float fever_temp = 100.4;

// // temperature calibration offset is +0.5 to +1.0 degree
// // to make axillary temperature comparible to ear or temporal.
// float temp_offset = 0.5;

// // Sensor read delay, in minutes
// int sensor_delay = 1;

// // Measuring your armpit temperature for a minimum of 12 minutes
// // is equivalent to measuring your core body temperature.
// int calibration_time = 12;

// // BLE transmit buffer
// char temperature_buf [8];

// // BLE Service
// BLEDfu  bledfu;  // OTA DFU service
// BLEDis  bledis;  // device information
// BLEUart bleuart; // uart over ble
// BLEBas  blebas;  // battery

// // Create the MCP9808 temperature sensor object
// Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();


// void startAdv(void)
// {
//   // Advertising packet
//   Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
//   Bluefruit.Advertising.addTxPower();

//   // Include bleuart 128-bit uuid
//   Bluefruit.Advertising.addService(bleuart);

//   // Secondary Scan Response packet (optional)
//   // Since there is no room for 'Name' in Advertising packet
//   Bluefruit.ScanResponse.addName();
  
//   /* Start Advertising
//    * - Enable auto advertising if disconnected
//    * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
//    * - Timeout for fast mode is 30 seconds
//    * - Start(timeout) with timeout = 0 will advertise forever (until connected)
//    * 
//    * For recommended advertising interval
//    * https://developer.apple.com/library/content/qa/qa1931/_index.html   
//    */
//   Bluefruit.Advertising.restartOnDisconnect(true);
//   Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
//   Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
//   Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
// }

// // callback invoked when central connects
// void connect_callback(uint16_t conn_handle)
// {
//   // Get the reference to current connection
//   BLEConnection* connection = Bluefruit.Connection(conn_handle);

//   char central_name[32] = { 0 };
//   connection->getPeerName(central_name, sizeof(central_name));

//   Serial.print("Connected to ");
//   Serial.println(central_name);
// }

// /**
//  * Callback invoked when a connection is dropped
//  * @param conn_handle connection where this event happens
//  * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
//  */
// void disconnect_callback(uint16_t conn_handle, uint8_t reason)
// {
//   (void) conn_handle;
//   (void) reason;

//   Serial.println();
//   Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
// }

// void setup() {

//   Serial.begin(115200);
//   delay(20000); 
//   Serial.println("Wearable BlueFruit Temperature Sensor");
//   Serial.println("-------------------------------------\n");


//   if (!tempsensor.begin(0x18)) {
//     Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
//     while (1);
//   }
//   Serial.println("Found MCP9808!");

//   // Sets the resolution of reading
//   tempsensor.setResolution(3);

//   // Configure BLE
//   // Setup the BLE LED to be enabled on CONNECT
//   // Note: This is actually the default behaviour, but provided
//   // here in case you want to control this LED manually via PIN 19
//   Bluefruit.autoConnLed(true);

//   // Config the peripheral connection with maximum bandwidth 
//   Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

//   Bluefruit.begin();
//   Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
//   Bluefruit.setName("Bluefruit52");
//   Bluefruit.Periph.setConnectCallback(connect_callback);
//   Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

//   // To be consistent OTA DFU should be added first if it exists
//   bledfu.begin();

//   // Configure and Start Device Information Service
//   bledis.setManufacturer("Adafruit Industries");
//   bledis.setModel("Bluefruit Feather52");
//   bledis.begin();

//   // Configure and Start BLE Uart Service
//   bleuart.begin();

//   // Start BLE Battery Service
//   blebas.begin();
//   blebas.write(100);

//   // Set up and start advertising
//   startAdv();

//   Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");

//   // initialize neopixel object
//   pixels.begin();

//   // set all pixel colors to 'off'
//   pixels.clear();
// }

// void loop() {

//   // wakes up MCP9808 - power consumption ~200 mikro Ampere
//   Serial.println("Wake up MCP9808");
//   tempsensor.wake();

//   // read and print the temperature
//   Serial.print("Temp: "); 
//   #if defined(TEMPERATURE_F)
//     float temp = tempsensor.readTempF();
//     // add temperature offset
//     temp += temp_offset;
//     Serial.print(temp);
//     Serial.println("*F.");
//   #elif defined(TEMPERATURE_C)
//     float temp = tempsensor.readTempC();
//     // add temperature offset
//     temp += temp_offset;
//     Serial.print(temp);
//     Serial.println("*C.");
//   #else
//     #warning "Must define TEMPERATURE_C or TEMPERATURE_F!"
//   #endif

//   // set NeoPixels to RED if fever_temp
//   if (temp >= fever_temp) {
//     pixels.setPixelColor(1, pixels.Color(255, 0, 0));
//     pixels.show();
//   }

//   // float to buffer
//   snprintf(temperature_buf, sizeof(temperature_buf) - 1, "%0.*f", 1, temp);

//   if (calibration_time == 0) {
//       Serial.println("Writing to UART");
//       // write to UART
//       bleuart.write(temperature_buf);
//   }
//   else {
//     Serial.print("Calibration time:");
//     Serial.println(calibration_time);
//     calibration_time-=1;
//   }

//   // shutdown MSP9808 - power consumption ~0.1 mikro Ampere
//   Serial.println("Shutting down MCP9808");
//   tempsensor.shutdown_wake(1);

//   // sleep for sensor_delay minutes
//   // NOTE: NRF delay() puts mcu into a low-power sleep mode
//   delay(1000*60*sensor_delay);
// }
