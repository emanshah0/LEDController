#include <Adafruit_NeoPixel.h>
#include "LEDController.h"

#define NUM_LEDS 50
#define TYPE NEO_GRB + NEO_KHZ800
#define RANDOMSHOT 300
#define PIN_OUT 5

float maxBrightness = 255;
float fadeSteps = maxBrightness / 15;

LEDController ledController(NUM_LEDS, PIN_OUT, TYPE);

void setup() {
  Serial.begin(115200);
  ledController.begin();
  ledController.startTwinkle(50, 30, 1);  // max brightness of 50, update interval of 30ms, and fade amount of 1
  ledController.setTwinkleChance(500);    // Set twinkle chance
  randomSeed(analogRead(0));              // Initialize random number generator
}

void loop() {
  uint8_t rgb[3] = { random(255), random(255), random(255) };

  if (random(RANDOMSHOT) == 0) {
    ledController.startShootingStar(0, 9, rgb, maxBrightness, fadeSteps);
  }

  if (random(RANDOMSHOT) == 0) {
    ledController.startShootingStar(10, 19, rgb, maxBrightness, fadeSteps);
  }

  if (random(RANDOMSHOT) == 0) {
    ledController.startShootingStar(20, 29, rgb, maxBrightness, fadeSteps);
  }

  if (random(RANDOMSHOT) == 0) {
    ledController.startShootingStar(30, 39, rgb, maxBrightness, fadeSteps);
  }

  if (random(RANDOMSHOT) == 0) {
    ledController.startParticleShot(40, 30, rgb, 50); 
  }
  
  ledController.update();
  delay(15);
}
