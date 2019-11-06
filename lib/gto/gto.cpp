#include <gto.h>
#include <Arduino.h>
#include <FastLED.h>
#include <stdlib.h>

/////////////////////////////////////////////
// FastLED light effects related functions //
/////////////////////////////////////////////

int a[9] = {0x9932CC, 0x8B0000, 0x0000CD, 0xFF4500, 0x006400, 0x87CEEB, 0xDAA520, 0x4B0082, 0xFF69B4};

void gto() {
    Serial.println();
}

void juggle_lib(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette) { // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= ColorFromPalette(currentPalette, dothue, 255);
    dothue += 32;
  }
}

void wild(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette) { // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  int i =1;
  for( int i = 0; i < 7; i++) {
    leds[beatsin16(i+7,0,19)] |= ColorFromPalette(currentPalette, dothue, 255);
    leds[beatsin16(i+7,20,39)] |= ColorFromPalette(currentPalette, dothue, 255);
    leds[beatsin16(i+7,40,59)] |= ColorFromPalette(currentPalette, dothue, 255);
    dothue += 32;
  }
}

void juggle_lib2(struct CRGB* leds, int NUM_LEDS) { // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void rainbow_lib(struct CRGB* leds, int NUM_LEDS, int gHue) {
  // FastLED's built-in ) {rainbow generator
    fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void learning_p(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette) {
  fadeToBlackBy(leds, NUM_LEDS, 200);
  int n =  random16(NUM_LEDS);
  int nl = n-1; 
  int nr = n+1;
  int nu = n+10;
  int nd = n-10;
  int rn = random8(255);
  leds[n] += ColorFromPalette(currentPalette, rn, 255);
  if (nl>-1){leds[nl] += ColorFromPalette(currentPalette, rn, 255);}
  if (nu<60){leds[nr] += ColorFromPalette(currentPalette, rn, 255);}
  if (nu<60){leds[nu] += ColorFromPalette(currentPalette, rn, 255);}
  if (nd>-1){leds[nd] += ColorFromPalette(currentPalette, rn, 255);}
  delay(125);
}

void learning_p2(struct CRGB* leds, int NUM_LEDS) {
  fadeToBlackBy(leds, NUM_LEDS, 200);
  int n =  random16(NUM_LEDS);
  int nl = n-1; 
  int nr = n+1;
  int nu = n+10;
  int nd = n-10;
  int rn = random8(255);
  leds[n] += CHSV(rn, 200, 255);
  if (nl>-1){leds[nl] += CHSV(rn, 200, 125);}
  if (nu<60){leds[nr] += CHSV(rn, 200, 125);}
  if (nu<60){leds[nu] += CHSV(rn, 200, 125);}
  if (nd>-1){leds[nd] += CHSV(rn, 200, 125);}
  delay(125);
}

void cubes(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette, uint8_t rn) {
  TBlendType    currentBlending;
  // currentPalette = OceanColors_p;
  currentBlending = LINEARBLEND;
  fadeToBlackBy(leds, NUM_LEDS, 200);
  // uint8_t color = 0;
  for (int j=0;j<3;j++){
    for (int i=0;i<5;i++){
      // int n = random8(NUM_LEDS);
      // int color = rand() % 16;
      // while (oldColor == color){color = a[rand() % 9];}
      leds[2*i+10*2*j] = ColorFromPalette(currentPalette, rn, 128);
      leds[2*i+1+10*2*j] = ColorFromPalette(currentPalette, rn, 128);
      leds[2*i+10+10*2*j] = ColorFromPalette(currentPalette, rn, 128);
      leds[2*i+11+10*2*j] = ColorFromPalette(currentPalette, rn, 128);
      delay(100);
      rn = rn+5;
    // oldColor = color;
    }
  }
}

void squares(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette) {
  int n = random16(NUM_LEDS);
  int rn = random8(255);
  leds[n] = ColorFromPalette(currentPalette, rn, 255);
  FastLED.show(); 
  delay(100);
  fadeToBlackBy(leds, NUM_LEDS, 50);
  FastLED.show(); 
  delay(100);
  fadeToBlackBy(leds, NUM_LEDS, 50);
  if((n+1)%10 != 0){leds[n+1] =  ColorFromPalette(currentPalette, rn, 128);}else{leds[n-9] =  ColorFromPalette(currentPalette, rn, 128);}
  if(n%10 != 0){leds[n-1] =  ColorFromPalette(currentPalette, rn, 128);}else{leds[n+9] =  ColorFromPalette(currentPalette, rn, 128);}
  if(n+10 < 60){leds[n+10] =  ColorFromPalette(currentPalette, rn, 128);}
  if(n-10 > -1){leds[n-10] =  ColorFromPalette(currentPalette, rn, 128);}
  if(((n+11)%10 != 0) && (n+11 < 60)){leds[n+11] =  ColorFromPalette(currentPalette, rn, 128);}else if(n+1 < 60){leds[n+1] |=  ColorFromPalette(currentPalette, rn, 128);}
  if(((n+9)%10 != 0) && (n+9 < 60)){leds[n+9] =  ColorFromPalette(currentPalette, rn, 128);}else if(n+19 < 60){leds[n+19] |=  ColorFromPalette(currentPalette, rn, 128);}
  if(((n-11)%10 != 0) && (n-11 > -1)){leds[n-11] =  ColorFromPalette(currentPalette, rn, 128);}else if(n-1 > -1){leds[n-1] |=  ColorFromPalette(currentPalette, rn, 128);}
  if(((n-9)%10 != 0) && (n-9 > -1)){leds[n-9] =  ColorFromPalette(currentPalette, rn, 128);}else if(n-19 > -1){leds[n-19] |=  ColorFromPalette(currentPalette, rn, 128);}
  FastLED.show(); 
  delay(35);
  fadeToBlackBy(leds, NUM_LEDS, 25);
  FastLED.show(); 
  delay(35);
  fadeToBlackBy(leds, NUM_LEDS, 25);
  FastLED.show(); 
  delay(35);
  fadeToBlackBy(leds, NUM_LEDS, 25);
  FastLED.show(); 
  delay(35);
  fadeToBlackBy(leds, NUM_LEDS, 25);
}


void up_down(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette, uint8_t rn) {
  for (int j=0; j<6; j++){
    for (int i=0; i<10; i++){
      leds[i+10*j] = ColorFromPalette(currentPalette, rn, 128);
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
      leds[i+10*j] = ColorFromPalette(currentPalette, rn, 128);
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

void fire_dep(struct CRGB* leds, int NUM_LEDS) {
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

void police_dep(struct CRGB* leds, int NUM_LEDS) {
  for (int j=0; j<10; j++){
    for (int i=0; i<6; i++){
      int red_spin = j+10*i;
      int blue_spin = 3+j+10*i;
      int white_spin = 6+j+10*i;
      leds[red_spin] = CRGB::DarkRed;
      leds[blue_spin] = CRGB::DarkBlue;
      leds[white_spin] = CRGB::White;
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

void maayan_rainbow(struct CRGB* leds, int NUM_LEDS) {
  int hue = 0;
  for (int k=0; k<10; k++){
    for (int j=0; j<10; j++){
      for (int i=0; i<6; i++){
        leds[j+10*i] |= CHSV(hue, 200, 125);
      }
    if (hue <255){hue += 32;} 
    else {hue=0;}
    }
  }
  FastLED.show();  
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