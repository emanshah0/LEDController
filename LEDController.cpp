#include "LEDController.h"

LEDController::LEDController(uint16_t numLeds, uint8_t pin, neoPixelType type)
  : strip(numLeds, pin, type), numLeds(numLeds), lastTwinkleUpdate(0), twinkleMaxBrightness(0), twinkleUpdateInterval(0), twinkleFadeAmount(0), twinkleChance(500) {
  brightness.resize(numLeds, 0);
  twinkleState.resize(numLeds, TwinkleState::OFF);
}

void LEDController::begin() {
  strip.begin();
  strip.clear();
  strip.show();
}

void LEDController::startShootingStar(uint16_t startLED, uint16_t endLED, uint8_t rgb[], uint8_t maxBrightness, uint8_t fadeSteps) {
  ShootingStar newStar;
  newStar.startLED = startLED;
  newStar.endLED = endLED;
  newStar.currentLED = startLED;
  newStar.color = strip.Color(rgb[0], rgb[1], rgb[2]);
  newStar.fadeSteps = fadeSteps;
  newStar.isAnimating = true;
  shootingStars.push_back(newStar);
}

void LEDController::lightUp(uint16_t startLED, uint16_t endLED, uint8_t grb[]) {
  strip.clear();
  for (int i = startLED; i < endLED; i++) {
    strip.setPixelColor(i, strip.Color(grb[1], grb[0], grb[2]));
  }
  strip.show();
}

void LEDController::update() {
  for (auto &star : shootingStars) {
    if (!star.isAnimating) continue;
    fadeLEDsBehind(star);

    if (star.currentLED <= star.endLED) {
      strip.setPixelColor(star.currentLED, star.color);
      strip.show();
      star.currentLED++;
    } else if (!allLEDsFaded(star)) {
      // Continue fading
    } else {
      star.isAnimating = false;  // End the animation once all LEDs have faded.
    }
  }

  // Remove finished shooting stars from the list
  shootingStars.erase(std::remove_if(shootingStars.begin(), shootingStars.end(),
                                     [](const ShootingStar &star) { return !star.isAnimating; }),
                      shootingStars.end());

  // Update the twinkle effect
  updateTwinkle();
}

bool LEDController::isLEDAnimating() {
  for (const auto &star : shootingStars) {
    if (star.isAnimating) return true;
  }
  return false;
}

void LEDController::fadeLEDsBehind(ShootingStar &star) {
  bool fadeNeeded = false;
  for (int i = star.startLED; i <= star.endLED; i++) {
    uint32_t currentColor = strip.getPixelColor(i);
    if (currentColor != 0) {
      fadeNeeded = true;
      uint8_t brightness = strip.getBrightness();
      if (brightness > star.fadeSteps) {
        brightness -= star.fadeSteps;
      } else {
        brightness = 0;
      }
      strip.setPixelColor(i, adjustBrightness(currentColor, brightness));
    }
  }
  if (fadeNeeded)
    strip.show();
}

bool LEDController::allLEDsFaded(ShootingStar &star) {
  for (int i = star.startLED; i <= star.endLED; i++) {
    if (strip.getPixelColor(i) != 0)
      return false;
  }
  return true;
}

void LEDController::setFadeSteps(uint8_t fadeSteps) {
  for (auto &star : shootingStars) {
    star.fadeSteps = fadeSteps;
  }
}

void LEDController::startTwinkle(uint8_t maxBrightness, uint16_t updateInterval, uint8_t fadeAmount) {
  this->twinkleMaxBrightness = maxBrightness;
  this->twinkleUpdateInterval = updateInterval;
  this->twinkleFadeAmount = fadeAmount;
  initializeTwinkle();
}

void LEDController::initializeTwinkle() {
  randomSeed(analogRead(0));  // Initialize random number generator
}

void LEDController::updateTwinkle() {
  unsigned long currentTime = millis();
  if (currentTime - lastTwinkleUpdate > twinkleUpdateInterval) {
    lastTwinkleUpdate = currentTime;

    for (int i = 0; i < numLeds; i++) {
      // Skip LEDs that are within a shooting star range
      if (isWithinShootingStarRange(i)) continue;

      switch (twinkleState[i]) {
        case TwinkleState::OFF:
          if (random(twinkleChance) == 0) {  // Start fading in
            twinkleState[i] = TwinkleState::FADING_IN;
            brightness[i] = 0;
          }
          break;
        case TwinkleState::FADING_IN:
          brightness[i] += twinkleFadeAmount;
          if (brightness[i] >= twinkleMaxBrightness) {
            brightness[i] = twinkleMaxBrightness;
            twinkleState[i] = TwinkleState::FADING_OUT;
          }
          break;
        case TwinkleState::FADING_OUT:
          if (brightness[i] > 0) {
            brightness[i] -= twinkleFadeAmount;
            if (brightness[i] < 0) brightness[i] = 0;
          } else {
            twinkleState[i] = TwinkleState::OFF;
          }
          break;
      }

      strip.setPixelColor(i, strip.Color(brightness[i], brightness[i], brightness[i]));
    }
    strip.show();
  }
}

void LEDController::setTwinkleChance(uint16_t chance) {
  this->twinkleChance = chance;
}

bool LEDController::isWithinShootingStarRange(uint16_t ledIndex) {
  for (const auto &star : shootingStars) {
    if (ledIndex >= star.startLED && ledIndex <= star.endLED && star.isAnimating) {
      return true;
    }
  }
  return false;
}

uint32_t LEDController::adjustBrightness(uint32_t color, uint8_t brightness) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  float factor = brightness / 255.0;

  r = static_cast<uint8_t>(r * factor);
  g = static_cast<uint8_t>(g * factor);
  b = static_cast<uint8_t>(b * factor);

  return strip.Color(r, g, b);
}
