#include <Arduino.h>
#include <SparkFunLSM6DSO.h>
#include "SparkFunLSM6DSO.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#include "Wire.h"

#define RED_PIN 32


// LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
 
// void setup() {
//  Serial.begin(9600);

// delay(500); 

// Wire.begin();
// delay(10);
// if( myIMU.begin() )
//   Serial.println("Ready.");
// else { 
//   Serial.println("Could not connect to IMU.");
//   Serial.println("Freezing");
// }

// if( myIMU.initialize(BASIC_SETTINGS) )
//   Serial.println("Loaded Settings.");

// // Bluetooth
// Serial.println("Starting BLE work!");

// BLEDevice::init("CS147_Team33");
// BLEServer *pServer = BLEDevice::createServer();
// BLEService *pService = pServer->createService(SERVICE_UUID);
// BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//                                       CHARACTERISTIC_UUID,
//                                       BLECharacteristic::PROPERTY_READ |
//                                       BLECharacteristic::PROPERTY_WRITE
//                                     );

// pCharacteristic->setValue("0");
// pService->start();
// BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
// pAdvertising->addServiceUUID(SERVICE_UUID);
// pAdvertising->setScanResponse(true);
// pAdvertising->setMinPreferred(0x0); 
// pAdvertising->setMinPreferred(0x12);
// BLEDevice::startAdvertising();
// Serial.println("Characteristic defined! Now you can read it in your phone!");
// }
// void loop() {
//   //Get all parameters
//   Serial.print("\nAccelerometer:\n");
//   Serial.print(" X = ");
//   Serial.println(myIMU.readFloatAccelX(), 3);
//   Serial.print(" Y = ");
//   Serial.println(myIMU.readFloatAccelY(), 3);
//   Serial.print(" Z = ");
//   Serial.println(myIMU.readFloatAccelZ(), 3);

//   Serial.print("\nGyroscope:\n");
//   Serial.print(" X = ");
//   Serial.println(myIMU.readFloatGyroX(), 3);
//   Serial.print(" Y = ");
//   Serial.println(myIMU.readFloatGyroY(), 3);
//   Serial.print(" Z = ");
//   Serial.println(myIMU.readFloatGyroZ(), 3);

//   Serial.print("\nThermometer:\n");
//   Serial.print(" Degrees F = ");
//   Serial.println(myIMU.readTempF(), 3);
  
//   delay(1000);
//   // delay(2000);
// }


class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     std::string value = pCharacteristic->getValue();
     std::string ON = "ON";
     std::string OFF = "OFF";
 
     if (value.length() > 0) {
        if (value == ON){
          // turn all lights off
          Serial.print("New value: ");
          for (int i = 0; i < value.length(); i++)
         Serial.print(value[i]);
          digitalWrite(RED_PIN, LOW);
        }else if (value == OFF){
         // turn all lights off
          digitalWrite(RED_PIN, HIGH);
        }
     }
   }
};
 
void setup() {
 Serial.begin(9600);

 // Configure LED pins and buzzer as outputs. 
  pinMode(RED_PIN, OUTPUT); 
  // turn all lights off
  digitalWrite(RED_PIN, LOW);
 
 Serial.println("1- Download and install an BLE scanner app in your phone");
 Serial.println("2- Scan for BLE devices in the app");
 Serial.println("3- Connect to MyESP32");
 Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
 Serial.println("5- See the magic =)");
 
 BLEDevice::init("MyESP32");
 BLEServer *pServer = BLEDevice::createServer();
 
 BLEService *pService = pServer->createService(SERVICE_UUID);
 
 BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE
                                      );
 
 pCharacteristic->setCallbacks(new MyCallbacks());
 
 pCharacteristic->setValue("Hello World");
 pService->start();
 
 BLEAdvertising *pAdvertising = pServer->getAdvertising();
 pAdvertising->start();
}
 
void loop() {
 delay(2000);
}
