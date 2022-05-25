#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>

void soilMoistureSensor();
void uvSensor();
int avgAnalogRead(int);

// FIXME:
#define MoistureSensor  39
#define UVIN 37 
#define REF_3v3 38

// never comment, needed for everything...
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
int x = tft.width() / 2;
int y = tft.height() / 2;

float moistValue = 0; 
int refValue = 0;
int uvValue = 0;

float outputVoltage = 0;
float uvIntensity = 0;


void setup() { 
  Serial.begin(9600); 
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically

  pinMode(UVIN, INPUT);
} 


void loop() {  
  soilMoistureSensor();
  uvSensor();
  diplay();
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

  uvValue = analogRead(UVIN);
  refValue = analogRead(REF_3v3);

  outputVoltage = 3.3 / refValue * uvValue;
  uvIntensity = (outputVoltage - 0.99) * (15) / (2.8 - 0.99);

  Serial.print("UV Sensor: ");
  Serial.println(uvValue);

  Serial.print("UV Intensity: ");
  Serial.println(uvIntensity);  
}

void display(){
  tft.setTextDatum(TC_DATUM); // Centre text on x,y
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically
  tft.setCursor(x, y, 2);

  tft.drawString("Soil Moisture Sensor:", x, y-50, 4);
  tft.drawFloat(moistValue, 0, x, y-25, 4);

  tft.drawString("UV:", x, y+25, 4);
  tft.drawFloat(uvValue, 0, x, y+50, 4);
}