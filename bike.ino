#include <Adafruit_NeoPixel.h>

#define LEFT_TURN_SIGNAL_BUTTON  2
#define RIGHT_TURN_SIGNAL_BUTTON 3
#define BRAKE_SWITCH 4
#define LEFT_TURN_SIGNAL_DATA  10
#define RIGHT_TURN_SIGNAL_DATA 11
#define NUM_LEDS 27
#define TURN_SIGNAL_IND 14
#define NIGHT_MODE_BRIGHTNESS 50

/* Setup for the LED strips */
Adafruit_NeoPixel stripL = Adafruit_NeoPixel(NUM_LEDS, RIGHT_TURN_SIGNAL_DATA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR = Adafruit_NeoPixel(NUM_LEDS, LEFT_TURN_SIGNAL_DATA, NEO_GRB + NEO_KHZ800);

/* Create color objects */
uint32_t off    = stripL.Color(0, 0, 0);
uint32_t red    = stripL.Color(255, 0, 0);
uint32_t orange = stripL.Color(128, 128, 0);

int leftIsOn=0, rightIsOn=0, brakeIsOn=0, stateLeft = 0, stateRight = 0;
long time = 0, flash = 0, debounce = 200;

void setup() {
  /* Init LED strips to off */
  stripL.begin();
  stripL.show();
  stripR.begin();
  stripR.show();

  /* Configure input pins with pullup resistors */
  pinMode(LEFT_TURN_SIGNAL_BUTTON, INPUT);
  pinMode(RIGHT_TURN_SIGNAL_BUTTON, INPUT);
  pinMode(BRAKE_SWITCH, INPUT);
  digitalWrite(LEFT_TURN_SIGNAL_BUTTON, HIGH);
  digitalWrite(RIGHT_TURN_SIGNAL_BUTTON, HIGH);
  digitalWrite(BRAKE_SWITCH, HIGH);

  /* Configure output pins */
  pinMode(LED_BUILTIN, OUTPUT);
}

/* Flashes the left turn signal */
void left_turn(){
  if(leftIsOn){
    if(flash % 334 < 167){
      digitalWrite(LED_BUILTIN, HIGH);
      for(int i=0; i<NUM_LEDS; i++){
        stripL.setPixelColor(i, orange);
      }
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      for(int i=0; i<NUM_LEDS; i++){
        stripL.setPixelColor(i, off);
      }
    }
  } else if(stripL.getPixelColor(NUM_LEDS-1) == orange){
    digitalWrite(LED_BUILTIN, LOW);
    for(int i=0; i<NUM_LEDS; i++){
      stripL.setPixelColor(i, off);
    }
  }
}

/* Flashes the right turn signal */
void right_turn(){
  if(rightIsOn){
    if(flash % 334 < 167){
      digitalWrite(LED_BUILTIN, HIGH);
      for(int i=0; i<NUM_LEDS; i++){
        stripR.setPixelColor(i, orange);
      }   
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      for(int i=0; i<NUM_LEDS; i++){
        stripR.setPixelColor(i, off);
      }
    }
  } else if(stripR.getPixelColor(NUM_LEDS-1) == orange){
    digitalWrite(LED_BUILTIN, LOW);
    for(int i=0; i<NUM_LEDS; i++){
      stripR.setPixelColor(i, off);
    }
  }
}


void loop() {
  /* Check if either of the turn signal buttons have pressed */
  if(digitalRead(LEFT_TURN_SIGNAL_BUTTON) == LOW){
    if(millis() - time > debounce && stateLeft == 0){
      leftIsOn = ! leftIsOn;
      rightIsOn = 0;
      time = millis();
    }
    stateLeft = 1;
  } else {
    stateLeft = 0;
  }
  if(digitalRead(RIGHT_TURN_SIGNAL_BUTTON) == LOW){
    if(millis() - time > debounce && stateRight == 0){
      rightIsOn = ! rightIsOn;
      leftIsOn = 0;
      time = millis();
    }
    stateRight = 1;
  } else {
    stateRight = 0;
  }
  if(digitalRead(RIGHT_TURN_SIGNAL_BUTTON) == LOW && digitalRead(LEFT_TURN_SIGNAL_BUTTON) == LOW){
    // Turn on night mode, this means the brake lights will constantly be on at a lower level
    stripL.setbrightness(NIGHT_MODE_BRIGHTNESS);
    stripR.setbrightness(NIGHT_MODE_BRIGHTNESS);
  }

  /* Call the turn signal methds to update the LED as needed */
  left_turn();
  right_turn();
  
  /* Turns the brakes on whenever the brake button is pressed, then turns them off when it is not pressed.
   *  Also, it respects the turn signals, and will not overwrite them.
   */
  if(digitalRead(BRAKE_SWITCH) == LOW){
    brakeIsOn = 1;
    for(int i=0; i<NUM_LEDS; i++){
      if(stripL.getPixelColor(i) == off){
        stripL.setPixelColor(i, red);
      }
      if(stripR.getPixelColor(i) == off){
        stripR.setPixelColor(i, red);
      }
    }
  } else if(brakeIsOn == 1){
    brakeIsOn = 0;
    for(int i=0; i<NUM_LEDS; i++){
      if(stripL.getPixelColor(i) == red){
        stripL.setPixelColor(i, off);
      }
      if(stripR.getPixelColor(i) == red){
        stripR.setPixelColor(i, off);
      }
    }
  }
  
  /* Update the color of the LEDs. Only call it once each iteration to prevent a visual flashing effect */
  stripL.show();
  stripR.show();

  /* Update the flash timer */
  flash = millis();
}
