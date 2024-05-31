#ifndef LEDController_h
#define LEDController_h

#include <Adafruit_NeoPixel.h>
#include <vector>

// Structure to represent a Shooting Star
struct ShootingStar {
  uint16_t startLED;
  uint16_t endLED;
  uint16_t currentLED;
  uint32_t color;
  uint8_t fadeSteps;
  bool isAnimating;
};

// Structure to represent a Particle Shot
struct ParticleShot {
  uint16_t startLED;
  uint16_t endLED;
  int16_t currentLED;
  uint32_t color;
  int16_t direction;
  uint16_t updateInterval;
  unsigned long lastUpdate;
  bool isAnimating;
};

// Enumeration to represent the state of twinkle
enum class TwinkleState {
  OFF,
  FADING_IN,
  FADING_OUT
};

class LEDController {
public:
  LEDController(uint16_t numLeds, uint8_t pin, neoPixelType type);
  void begin();
  void startShootingStar(uint16_t startLED, uint16_t endLED, uint8_t rgb[], uint8_t maxBrightness, uint8_t fadeSteps);
  void startParticleShot(uint16_t startLED, uint16_t endLED, uint8_t rgb[], uint16_t updateInterval);
  void update();
  bool isLEDAnimating();
  void lightUp(uint16_t startLED, uint16_t endLED, uint8_t grb[]);
  void setFadeSteps(uint8_t fadeSteps);
  void startTwinkle(uint8_t maxBrightness, uint16_t updateInterval, uint8_t fadeAmount);
  void updateTwinkle();
  void setTwinkleChance(uint16_t chance);

private:
  Adafruit_NeoPixel strip;                    // NeoPixel strip object
  uint16_t numLeds;                           // Number of LEDs in the strip
  std::vector<ShootingStar> shootingStars;    // Vector to store shooting stars
  std::vector<ParticleShot> particleShots;    // Vector to store particle shots
  std::vector<int> brightness;                // Brightness levels for twinkle effect
  std::vector<TwinkleState> twinkleState;     // State of each LED for twinkle effect
  
  unsigned long lastTwinkleUpdate;            // Last update time for twinkle effect
  uint8_t twinkleMaxBrightness;               // Maximum brightness for twinkle effect
  uint16_t twinkleUpdateInterval;             // Update interval for twinkle effect
  uint8_t twinkleFadeAmount;                  // Fade amount for twinkle effect
  uint16_t twinkleChance;                     // Chance of twinkle effect occurring

  void fadeLEDsBehind(ShootingStar &star);
  bool allLEDsFaded(ShootingStar &star);
  void initializeTwinkle();
  bool isWithinShootingStarRange(uint16_t ledIndex);
  uint32_t adjustBrightness(uint32_t color, uint8_t brightness);
};

#endif
