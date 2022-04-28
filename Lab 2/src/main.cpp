#include <Arduino.h>
#include <Wire.h>

// connected LED pins FIXME
#define RED_PIN 32
#define YELLOW_PIN 33
#define GREEN_PIN 25

// connected buzzer FIXME
#define BUZZ_PIN 10

//connected touch button FIXME
#define TOUCH_PIN 11

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

int tl_state;           // Traffic light state.
unsigned long tl_timer; // Traffic light timer.
unsigned long tl_buzzer;  // Traffic light buzzer timer.
short buzz; // to buzz or not to buzz.

void setup()
{
  Serial.begin(9600); // baudrate 9600
  // Configure LED pins and buzzer as outputs. 
  pinMode(RED_PIN, OUTPUT); 
  pinMode(YELLOW_PIN, OUTPUT); 
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  // configure touch button as input.
  pinMode(TOUCH_PIN, INPUT_PULLUP);

  // turn all lights off
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  // Initial state for states and timers.. 
  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;

  // Intialize buzzing.
  buzz = 0;
  tl_buzzer = millis() + 250;
  digitalWrite(BUZZ_PIN, HIGH);

  Serial.print("Setup Complete.");
}

void loop()
{
  int button_state = LOW;

  // TL state machine
  switch (tl_state)
  {
  case RED_STATE:
    Serial.print("RED state.");
    // Turn red light on.
    digitalWrite(RED_PIN,  HIGH);

    // buzzer check
    if (buzz == 1 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 250;
      digitalWrite(BUZZ_PIN, LOW);
      buzz = 0;
    } else if ( buzz == 0 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 250;
      digitalWrite(BUZZ_PIN, HIGH);
      buzz = 1;
    }

    /*Timer expired*/
    if (tl_timer <= millis()){ 
        // Turn red light off and buzzer.
        digitalWrite(RED_PIN, LOW);
        digitalWrite(BUZZ_PIN, LOW);

        // Set timer for red-yellow state.
        tl_state = RED_YELLOW_STATE;
        tl_timer = millis() + RED_YELLOW_MILLIS;
    }

    break;
  case RED_YELLOW_STATE:
    digitalWrite(BUZZ_PIN, HIGH);
    Serial.print("RED YELLOW state.");
    // yellow light
    digitalWrite(YELLOW_PIN, HIGH);
    // red light
    digitalWrite(RED_PIN, HIGH);

    if (tl_timer <= millis()){
        // Turn red-yellow light off.
        digitalWrite(RED_PIN, LOW);
        digitalWrite(YELLOW_PIN, LOW);

        // Set timer for green state.
        tl_state = GREEN_STATE;
        tl_timer = millis() + GREEN_MILLIS;

        // turn on the buzzer
        buzz = 1;
        tl_buzzer = millis() + 500;
        digitalWrite(BUZZ_PIN, HIGH);
    }
  
    break; 
  case YELLOW_STATE:
    digitalWrite(BUZZ_PIN, HIGH);
    Serial.print("YELLOW state.");
    // yellow light
    digitalWrite(YELLOW_PIN, HIGH);    

    if (tl_timer <= millis()){
      // Turn yellow light off.
      digitalWrite(YELLOW_PIN, LOW);

      // Set timer for red state.
      tl_state = RED_STATE;
      tl_timer = millis() + RED_MILLIS;

      // turn on the buzzer
      buzz = 1;
      tl_buzzer = millis() + 250;
      digitalWrite(BUZZ_PIN, HIGH);
    }

    break;
  case GREEN_STATE:
    Serial.print("GREEN state.");
    // Turn green light on.
    digitalWrite(GREEN_PIN, HIGH);
    // Detect touch - button pressed.
    button_state = digitalRead(TOUCH_PIN);

    // buzzer check
    if (buzz == 1 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 1500;
      digitalWrite(BUZZ_PIN, HIGH);
      buzz = 0;
    } else if (buzz == 0 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 500;
      digitalWrite(BUZZ_PIN, LOW);
      buzz = 1;
    }

    /*Timer expired AND touch-button was pressed*/
    if (tl_timer <= millis() && button_state == HIGH)
    {
      // must wait 5 seconds after button press
      delay(5000);  // while loop for buzzing check new tl_timer

      // Turn green light off and buzzer off.
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BUZZ_PIN, LOW);

      // Set timer for yellow 
      tl_state = YELLOW_STATE;
      tl_timer = millis() + YELLOW_MILLIS;
    }

    break;
  }
}


// Backup just in case

/*
// Stephanie Estanislao
// Joshua Gomes

#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>

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
  Serial.begin(9600); // baudrate 9600
  // Configure LED pins and buzzer as outputs. 
  pinMode(RED_PIN, OUTPUT); 
  pinMode(YELLOW_PIN, OUTPUT); 
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  // configure touch button as input.
  pinMode(TOUCH_PIN, INPUT_PULLUP);

  // turn all lights off
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  // Initial state for states and timers.. 
  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;

  // Intialize buzzing.
  buzz = 0;
  tl_buzzer = millis() + 250;
  digitalWrite(BUZZ_PIN, HIGH);

  Serial.print("Setup Complete.");
}

void loop()
{
  int button_state = LOW;

  // TL state machine
  switch (tl_state)
  {
  case RED_STATE:
    Serial.print("RED state.");
    // Turn red light on.
    digitalWrite(RED_PIN,  HIGH);

    // buzzer check
    if (buzz == 1 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 250;
      digitalWrite(BUZZ_PIN, LOW);
      buzz = 0;
    } else if ( buzz == 0 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 250;
      digitalWrite(BUZZ_PIN, HIGH);
      buzz = 1;
    }

    if (tl_timer <= millis()){ 
        // Turn red light off and buzzer.
        digitalWrite(RED_PIN, LOW);
        digitalWrite(BUZZ_PIN, LOW);

        // Set timer for red-yellow state.
        tl_state = RED_YELLOW_STATE;
        tl_timer = millis() + RED_YELLOW_MILLIS;
    }

    break;
  case RED_YELLOW_STATE:
    digitalWrite(BUZZ_PIN, HIGH);
    Serial.print("RED YELLOW state.");
    // yellow light
    digitalWrite(YELLOW_PIN, HIGH);
    // red light
    digitalWrite(RED_PIN, HIGH);

    if (tl_timer <= millis()){
        // Turn red-yellow light off.
        digitalWrite(RED_PIN, LOW);
        digitalWrite(YELLOW_PIN, LOW);

        // Set timer for green state.
        tl_state = GREEN_STATE;
        tl_timer = millis() + GREEN_MILLIS;

        // turn on the buzzer
        buzz = 1;
        tl_buzzer = millis() + 500;
        digitalWrite(BUZZ_PIN, HIGH);
    }
  
    break; 
  case YELLOW_STATE:
    digitalWrite(BUZZ_PIN, HIGH);
    Serial.print("YELLOW state.");
    // yellow light
    digitalWrite(YELLOW_PIN, HIGH);    

    if (tl_timer <= millis()){
      // Turn yellow light off.
      digitalWrite(YELLOW_PIN, LOW);

      // Set timer for red state.
      tl_state = RED_STATE;
      tl_timer = millis() + RED_MILLIS;

      // turn on the buzzer
      buzz = 1;
      tl_buzzer = millis() + 250;
      digitalWrite(BUZZ_PIN, HIGH);
    }

    break;
  case GREEN_STATE:
    Serial.print("GREEN state.");
    // Turn green light on.
    digitalWrite(GREEN_PIN, HIGH);
    // Detect touch - button pressed.
    button_state = digitalRead(TOUCH_PIN);

    // buzzer check
    if (buzz == 1 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 1500;
      digitalWrite(BUZZ_PIN, HIGH);
      buzz = 0;
    } else if (buzz == 0 && tl_buzzer <= millis()){
      tl_buzzer = millis() + 500;
      digitalWrite(BUZZ_PIN, LOW);
      buzz = 1;
    }

    if (tl_timer <= millis() && button_state == HIGH)
    {
      // must wait 5 seconds after button press
      delay(5000);  // while loop for buzzing check new tl_timer

      // Turn green light off and buzzer off.
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BUZZ_PIN, LOW);

      // Set timer for yellow 
      tl_state = YELLOW_STATE;
      tl_timer = millis() + YELLOW_MILLIS;
    }

    break;
  }
}

*/
