#include <Arduino.h>
#include <Wire.h>

/*
// We assigned a name LED pin to pin number 22
const int BUZZER_PIN = 13; 
// this will assign the name PushButton to pin numer 15
const int PushButton = 15;
// This Setup function is used to initialize everything 
void setup()
{
// This statement will declare pin 22 as digital output 
pinMode(BUZZER_PIN, OUTPUT);
// This statement will declare pin 15 as digital input 
pinMode(PushButton, INPUT);
}

void loop()

{
// digitalRead function stores the Push button state 
// in variable push_button_state
int Push_button_state = digitalRead(PushButton);
// if condition checks if push button is pressed
// if pressed LED will turn on otherwise remain off 
if ( Push_button_state == HIGH )
{ 
digitalWrite(BUZZER_PIN, LOW); 
}
else 
{
digitalWrite(BUZZER_PIN, HIGH); 
}
}
// Traffic lights no buzzer or button
digitalWrite(RED_PIN,HIGH); //the red LED lights up
digitalWrite(YELLOW_PIN,LOW); 
digitalWrite(GREEN_PIN,LOW);
delay(3000);
digitalWrite(RED_PIN,LOW); 
digitalWrite(YELLOW_PIN,HIGH); //the green LED lights up
digitalWrite(5,LOW); 
delay(3000);
digitalWrite(RED_PIN,LOW); 
digitalWrite(YELLOW_PIN,LOW); 
digitalWrite(GREEN_PIN,HIGH); // the yellow LED lights up
delay(1000);
*/
// connected LED pins FIXME
#define RED_PIN 22
#define YELLOW_PIN 17
#define GREEN_PIN 25

// connected buzzer FIXME
#define BUZZ_PIN 13

//connected touch button FIXME
#define TOUCH_PIN 15

// Hard coded enumerator 
#define RED_STATE 0
#define RED_YELLOW_STATE 1
#define YELLOW_STATE 2
#define GREEN_STATE 3

// time in each state
#define RED_MILLIS 10000
#define RED_YELLOW_MILLIS 2000
#define YELLOW_MILLIS 2000
#define GREEN_MILLIS 5000

int tl_state ;           // Traffic light state.
unsigned long tl_timer; // Traffic light timer.
unsigned long tl_buzzer;  // Traffic light buzzer timer.
int buzz; // to buzz or not to buzz...that is the question
int ledState = LOW;
unsigned long previousMillis = 0;

void setup()
{
    // Configure LED pins and buzzer as outputs. 
    pinMode(RED_PIN, OUTPUT); 
    pinMode(YELLOW_PIN, OUTPUT); 
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BUZZ_PIN, OUTPUT);

    // configure touch button as input.
    pinMode(TOUCH_PIN, INPUT);

    // turn all lights off
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);

    // Initial state for states and timers.. 
    tl_state = 0;
    tl_timer = 10000;

    // Intialize buzzing.
    buzz = 0;
    tl_buzzer = 250;
    digitalWrite(BUZZ_PIN, HIGH);
    
}

void loop(){
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= RED_MILLIS) {
      // save the last time you blinked the LED
        previousMillis = currentMillis;

        if (ledState == LOW) {
          ledState = HIGH;
        } else {
          ledState = LOW;
        }

    // set the LED with the ledState of the variable:
      digitalWrite(RED_PIN, ledState);
      }
}
