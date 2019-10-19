#ifndef lightshow_h
#define lightshow_h

#include <FastLED.h>
#include <Arduino.h>

int wrap(int step, int NUM_LEDS);
void ripple(CRGB *leds, int NUM_LEDS);
void blendwave(CRGB *LEDS, int NUM_LEDS);
void rainbow_march(CRGB *leds, int NUM_LEDS, uint8_t thisdelay, uint8_t deltahue);
void noise16_2(CRGB *leds, int NUM_LEDS);
#endif