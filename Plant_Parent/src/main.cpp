#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <string>
#include <iostream>
#include <sstream>


int soilMoistureSensor();
int uvSensor();
void display();


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

// WiFi set up

//char ssid[] = "";    // your network SSID (name) FIXME
//char pass[] = ""; // your network password (use for WPA, or use as key for WEP) FIXME

// Name of the server we want to connect to
const char kHostname[] = "44.202.225.24";  // FIXME
// Path to download (this is the bit after the hostname in the URL
// that you want to download
std::string kPath = "/?temp="; // FIXME

const int kport = 5000;

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;


void setup() { 
  Serial.begin(9600);
  delay(10);
  Serial.println(WiFi.macAddress());
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("Connecting....");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically

  pinMode(UVIN, INPUT);
} 


void loop() { 
  // Soil Moisture
  moistValue = soilMoistureSensor();
  Serial.print("Soil Moisture Sensor: ");
  Serial.println(moistValue);
  // UV Sensor
  uvValue = uvSensor();
  Serial.print("UV Sensor: ");
  Serial.println(uvValue);
  // Print to ESP32
  display();
  delay(30);

    int err =0;
  
  WiFiClient c;
  HttpClient http(c);
  std::ostringstream ss;
  std::ostringstream uu;
  ss << moistValue;
  uu << uvValue;
  std::string s(ss.str());
  std::string uv(uu.str());
  kPath = "/?Soil_Moisture= " + s + "&UV_Sensor= " + uv;
  const char* path = kPath.c_str();
  err = http.get(kHostname, kport, path);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
               
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  // And just stop, now that we've tried a download
  while(1);
} 


int soilMoistureSensor(){
  // soil moisture
  for (int i = 0; i <= 100; i++) 
  { 
    moistValue = moistValue + analogRead(MoistureSensor); 
    delay(1); 
  } 
  moistValue = moistValue/100.0; // for stability
  return moistValue;
}


int uvSensor(){

  uvValue = analogRead(UVIN);
  refValue = analogRead(REF_3v3);

  outputVoltage = 3.3 / refValue * uvValue;
  uvIntensity = (outputVoltage - 0.99) * (15) / (2.8 - 0.99);

  

  Serial.print("UV Intensity: ");
  Serial.println(uvIntensity);

  return uvValue;
}

void display(){
  tft.setTextDatum(TC_DATUM); // Centre text on x,y
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically
  tft.setCursor(x, y, 2);

  tft.drawString("Soil Moisture Sensor:", x, y-50, 4);
  //tft.drawFloat(moistValue, 0, x, y-25, 4);

  tft.drawString("UV:", x, y+25, 4);
  //tft.drawFloat(uvValue, 0, x, y+50, 4);
  tft.drawFloat(0, 0, x, y, 4);
  delay(500);
}
