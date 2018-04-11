#include <Adafruit_NeoPixel.h>

#define LEFT 2
#define RIGHT 3
#define BRAKE 4
#define PIN 7
#define LEDS 300

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t off = strip.Color(0, 0, 0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t orange = strip.Color(128, 128, 0);

int left=0, right=0, brake=0;
long time = 0;
int debounce = 200;

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(BRAKE, INPUT);
  digitalWrite(LEFT, HIGH);
  digitalWrite(RIGHT, HIGH);
  digitalWrite(BRAKE, HIGH);
}

void loop() {
  if(digitalRead(LEFT) == LOW && millis() - time > debounce){
    left = !left;
  }
  if(digitalRead(RIGHT) == LOW && millis() - time > debounce){
    right = !right;
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
    }
  } else if(brake == 1){
    brake = 0;
    for(int i=0; i<LEDS; i++){
      if(strip.getPixelColor(i) == red){
        strip.setPixelColor(i, off);
      }
    }
  }
  
  strip.show(); // only call to strip.show(), updates all the leds at the same time
  time = millis();
}

void left_turn(){
  if(left){
    // flash turn signal
    if(millis() % 1000 < 500){
      for(int i=0; i<LEDS/2; i++){
        strip.setPixelColor(i, orange);
      }   
    } else {
      for(int i=0; i<LEDS/2; i++){
        strip.setPixelColor(i, off);
      }
    }
  } else if(strip.getPixelColor(0) == orange){
    for(int i=0; i<LEDS/2; i++){
      strip.setPixelColor(i, off);
    }
  }
}

void right_turn(){
  if(right){
    // flash turn signal
    if(millis() % 1000 < 500){
      for(int i=LEDS/2; i<LEDS; i++){
        strip.setPixelColor(i, orange);
      }   
    } else {
      for(int i=LEDS/2; i<LEDS; i++){
        strip.setPixelColor(i, off);
      }
    }
  } else if(strip.getPixelColor(0) == orange){
    for(int i=LEDS/2; i<LEDS; i++){
      strip.setPixelColor(i, off);
    }
  }
}

