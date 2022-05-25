#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>

// FIXME:
#define MoistureSensor 0 // FIXME: replace with sensor pin
#define UVOUT 1 //FIXME
#define REF_3v3 2 //FIXME

// never comment, needed for everything...
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
int x = tft.width() / 2;
int y = tft.height() / 2;

float moistValue = 0; 
int refValue = 0;
int uvValue = 0;

float outputVoltage = 0;
float uvIntesity = 0;


void setup() { 
  Serial.begin(9600); 
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically

  pinMode(UVOUT, INPUT);
} 


void loop() {  
  soilMoistureSensor();
  uvSensor();
  delay(30); 
} 

void soilMoistureSensor(){
  // soil moisture
  for (int i = 0; i <= 100; i++) 
  { 
    moistValue = moistValue + analogRead(MoistureSensor); 
    delay(1); 
  } 
  moistValue = moistValue/100.0; // for stability
  Serial.print("Soil Moisture Sensor: ");
  Serial.println(moistValue); 

}

void uvSensor(){
  // uv sensor
  uvValue = avgAnalogRead(UVOUT);
  refValue = avgAnalogRead(REF_3v3);

  outputVoltage = 3.3 / refValue * uvValue;
  uvIntesity = (outputVoltage - 0.99) * (15) / (2.8 - 0.99);

  Serial.print("UV Sensor: ");
  Serial.println(uvValue);

  Serial.print("UV Intensity: ");
  Serial.println(uvIntesity);  
}

int avgAnalogRead(int pin){
  byte numReadings = 0;
  unsigned int runningValue = 0;

  for (int x=0; x<numReadings; x++)
    runningValue += analogRead(pin);
  runningValue /= numReadings;

  return (runningValue);
}