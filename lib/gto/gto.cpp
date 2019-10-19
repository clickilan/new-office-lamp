#include <gto.h>
#include <Arduino.h>
#include <FastLED.h>

/////////////////////////////////////////////
// FastLED light effects related functions //
/////////////////////////////////////////////

void gto() {
    Serial.println();
}

void juggle_lib(struct CRGB* leds, int NUM_LEDS) { // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void rainbow_lib(struct CRGB* leds, int NUM_LEDS, int gHue) {
  // FastLED's built-in rainbow generator
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void learning_p(struct CRGB* leds, int NUM_LEDS) {
  fadeToBlackBy(leds, NUM_LEDS, 500);
  leds[ random16(NUM_LEDS) ] += CHSV(random8(255), 200, 255);
}

void up_down(struct CRGB* leds, int NUM_LEDS) {
  for (int j=0; j<6; j++){
    for (int i=0; i<10; i++){
      leds[i+10*j] = CRGB::Crimson;
    }
    FastLED.show();
    delay(100);
    fadeToBlackBy(leds,NUM_LEDS, 150);
    // for (int i=0; i<10; i++){
    //   leds[i+10*j] = CRGB::Black;
    // }
    FastLED.show();
  }
  for (int j=4; j>0; j--){
    for (int i=0; i<10; i++){
      leds[i+10*j] = CRGB::Crimson;
    }
    FastLED.show();
    delay(100);
    fadeToBlackBy(leds,NUM_LEDS, 150);
    // for (int i=0; i<10; i++){
    //   leds[i+10*j] = CRGB::Black;
    // }
    FastLED.show();
  }
  
}

void learning_p2(struct CRGB* leds, int NUM_LEDS) {
  for (int j=0; j<10; j++){
    for (int i=0; i<6; i++){
      leds[j+10*i] = CRGB::DarkRed;
    }
    FastLED.show();
    delay(30);
    fadeToBlackBy(leds,NUM_LEDS, 150);
    // for (int i=0; i<10; i++){
    //   leds[i+10*j] = CRGB::Black;
    // }
    FastLED.show();
  }
  
}


void addGlitter_lib(struct CRGB* leds, int NUM_LEDS, fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbowWithGlitter_lib(struct CRGB* leds, int NUM_LEDS, int gHue) { // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow_lib(leds, NUM_LEDS, gHue);
  addGlitter_lib(leds, NUM_LEDS, 80);
}