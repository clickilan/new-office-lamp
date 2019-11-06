#ifndef GTO_H
#define GTO_H

/////////////////////////////////////////////
// FastLED light effects related functions //
/////////////////////////////////////////////

#include <Arduino.h>
#include <FastLED.h>

void gto();
void juggle_lib(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette);
void wild(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette);
void rainbow_lib(struct CRGB* leds, int NUM_LEDS, int gHue);
void addGlitter_lib(struct CRGB* leds, int NUM_LEDS, fract8 chanceOfGlitter);
void rainbowWithGlitter_lib(struct CRGB* leds, int NUM_LEDS, int gHue);
void learning_p(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette);
void fire_dep(struct CRGB* leds, int NUM_LEDS);
void up_down(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette, uint8_t rn);
void police_dep(struct CRGB* leds, int NUM_LEDS);
void maayan_rainbow(struct CRGB* leds, int NUM_LEDS);
void cubes(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette, uint8_t rn);
void squares(struct CRGB* leds, int NUM_LEDS, CRGBPalette16 currentPalette);
#endif