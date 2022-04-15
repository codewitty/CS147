#include <Arduino.h>
#include <TFT_eSPI.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

// never comment, needed for everything...
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
int x = tft.width() / 2;
int y = tft.height() / 2;
// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  9

// COUNTING ****
// unsigned long startTime; // long originally

// TOUCH SENSORS I2C ****
// // Or...Use I2C, with reset pin
// Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

// TOUCH SENSORS SOFTWARE SPI ****
// CS pin is used for software or hardware SPI
#define CAP1188_CS  10
// These are defined for software SPI, for hardware SPI, check your 
// board's SPI pins in the Arduino documentation
#define CAP1188_MOSI  11
#define CAP1188_MISO  12
#define CAP1188_CLK  13

// Or.. Software SPI: clock, miso, mosi, cs, reset
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_CLK, CAP1188_MISO, CAP1188_MOSI, CAP1188_CS, CAP1188_RESET);

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically

  // counter setup
  // Serial.begin(9600);
  // startTime = millis();

  // Initialize the sensor, if using i2c you can pass in the i2c address
  //if (!cap.begin(0x29)) {
  if (!cap.begin()) {
    tft.println("CAP1188 not found");
    while (1);
  }
  tft.println("CAP1188 found!");

  
}

// void counter(){
//   tft.setTextFont(7); // Select the orginal small GLCD font by using NULL or GLCD
//   //tft.setTextDatum(TC_DATUM); // Centre text on x,y position
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically
//   unsigned long duration = (millis() - startTime) / 1000;
//   tft.setCursor(x, y, 4);
//   tft.print((int)duration); // print to display
//   delay(50);
// }

void touchedSensors(){
  uint8_t touched = cap.touched();
  
  tft.setTextDatum(TC_DATUM); // Centre text on x,y
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);  // Adding a black background colour erases previous text automatically
  tft.setCursor(x, y, 2);

  if (touched == 0){
    //no touch
    tft.drawFloat(0, 0, x, y, 4);
    tft.drawString("caps", x, y+25, 4);
    delay(50);
    return;
  }

  int count = 0;
  for (uint8_t i=0; i<9; i++){
    if (touched & (1 << i)){
      count++;
    }
  }

  tft.drawFloat(count, 0, x, y, 4);
  tft.drawString("caps", x, y+25, 4);
  delay(50);
}

void loop() {
  //counter();
  
  touchedSensors();

 
}