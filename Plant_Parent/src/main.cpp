#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>
#include <iostream>
#include <sstream>
#include <Arduino_JSON.h>


int soilMoistureSensor();
int uvSensor();
void display();


#define MoistureSensor  39
#define UVIN 37 
#define REF_3v3 38

float moistValue = 0; 
int refValue = 0;
int uvValue = 0;

float outputVoltage = 0;
float uvIntensity = 0;

// WiFi set up

//char ssid[] = "dunder_mifflin";    // your network SSID (name) FIXME
//char pass[] = "thatswhatshesaid69"; // your network password (use for WPA, or use as key for WEP) FIXME

// char ssid[] = "UCInet Mobile Access";    // your network SSID (name) FIXME
// char pass[] = ""; // your network password (use for WPA, or use as key for WEP) FIXME

char ssid[] = "Uba Uba Guata";    // your network SSID (name) FIXME
char pass[] = "L0QueY0Quier0EsUnaGata"; // your network password (use for WPA, or use as key for WEP) FIXME


// Name of the server we want to connect to
const char kHostname[] = "192.168.10.174";  // FIXME
const char* serverName = "http://192.168.10.174:5000";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
std::string kPath = "/?temp="; // FIXME

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

const int kport = 5000;

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

WiFiClient client;
HTTPClient http;


void setup() { 
  Serial.begin(9600);
  delay(1000);
  //WiFi.mode(WIFI_STA);
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }

  Serial.println();
  Serial.println();
  Serial.println(WiFi.macAddress());
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      Serial.println(WiFi.status());
      delay(500);
      Serial.print("Connecting....");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());

  pinMode(UVIN, INPUT);
} 


void loop() { 
  // Soil Moisture
  moistValue = soilMoistureSensor();
  Serial.println();
  Serial.print("\n");
  Serial.print("Soil Moisture Sensor: ");
  Serial.println(moistValue);
  // UV Sensor
  uvValue = uvSensor();
  Serial.print("UV Sensor: ");
  Serial.println(uvValue);
  delay(30);

  //int err =0;
  std::ostringstream ss;
  std::ostringstream uu;
  
  ss << moistValue;
  uu << uvValue;

  std::string s(ss.str());
  std::string uv(uu.str());
  Serial.println("Just Before sending Soil Moisture Sensor: ");
  Serial.println(moistValue);
  kPath = "Soil_Moisture=" + s + "&UV_Sensor=" + uv;
  const char* path = kPath.c_str();
  http.begin(client, serverName);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Data to send with HTTP POST
  //String httpRequestData = "&Soil_Moisture=" + s + "&UV_Sensor=" + uv;           
  // Send HTTP POST request
  int httpResponseCode = http.POST(path);
     
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
        
  // Free resources
  //http.end();
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
