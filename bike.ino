#include <Adafruit_NeoPixel.h>

#define LEFT  2
#define RIGHT 3
#define BRAKE 4
#define RL 10
#define LL 11
#define LEDS 27
#define LED 14

// Setup for the LED strip
Adafruit_NeoPixel strip  = Adafruit_NeoPixel(LEDS, RL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR = Adafruit_NeoPixel(LEDS, LL, NEO_GRB + NEO_KHZ800);

uint32_t off = strip.Color(0, 0, 0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t orange = strip.Color(128, 128, 0);

int left=0, right=0, brake=0;
long time = 0, flash = 0, debounce = 200;

void setup() {
  strip.begin();
  strip.show();
  stripR.begin();
  stripR.show();
  pinMode(LEFT, INPUT); // setup pins as inputs
  pinMode(RIGHT, INPUT);
  pinMode(BRAKE, INPUT);
  digitalWrite(LEFT, HIGH); // enable pull up resistors
  digitalWrite(RIGHT, HIGH);
  digitalWrite(BRAKE, HIGH);

  pinMode(LED, OUTPUT);
}

void left_turn(){
  if(left){
    // flash turn signal
    if(flash % 500 < 250){
      digitalWrite(LED, HIGH);
      for(int i=0; i<LEDS; i++){
        strip.setPixelColor(i, orange);
      }
    } else {
      digitalWrite(LED, LOW);
      for(int i=0; i<LEDS; i++){
        strip.setPixelColor(i, off);
      }
    }
  } else if(strip.getPixelColor(LEDS-1) == orange){
    digitalWrite(LED, LOW);
    for(int i=0; i<LEDS; i++){
      strip.setPixelColor(i, off);
    }
  }
}

void right_turn(){
  if(right){
    // flash turn signal
    if(flash % 500 < 250){
      digitalWrite(LED, HIGH);
      for(int i=0; i<LEDS; i++){
        stripR.setPixelColor(i, orange);
      }   
    } else {
      digitalWrite(LED, LOW);
      for(int i=0; i<LEDS; i++){
        stripR.setPixelColor(i, off);
      }
    }
  } else if(stripR.getPixelColor(LEDS-1) == orange){
    digitalWrite(LED, LOW);
    for(int i=0; i<LEDS; i++){
      stripR.setPixelColor(i, off);
    }
  }
}


void loop() {
  if(digitalRead(LEFT) == LOW && millis() - time > debounce){
    left = !left;
    time = millis();
  }
  if(digitalRead(RIGHT) == LOW && millis() - time > debounce){
    right = !right;
    time = millis();
  }
  left_turn();
  right_turn();
  
  /* Turns the brakes on whenever the brake button is pressed, then turns them off when it is not pressed.
   *  Also, it respects the turn signals, and will not interfere with them turning the lights orange.
   */
  if(digitalRead(BRAKE) == LOW){
    brake = 1;
    for(int i=0; i<LEDS; i++){
      if(strip.getPixelColor(i) == off){
        strip.setPixelColor(i, red);
      }
      if(stripR.getPixelColor(i) == off){
        stripR.setPixelColor(i, red);
      }
    }
  } else if(brake == 1){
    brake = 0;
    for(int i=0; i<LEDS; i++){
      if(strip.getPixelColor(i) == red){
        strip.setPixelColor(i, off);
      }
      if(stripR.getPixelColor(i) == red){
        stripR.setPixelColor(i, off);
      }
    }
  }
  
  strip.show(); // only call to strip.show(), updates all the leds at the same time
  stripR.show();
  flash = millis();
}
