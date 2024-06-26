### README.md

# LEDController Library

The `LEDController` library is designed to control a NeoPixel LED strip, providing animations such as shooting stars and twinkle effects. This documentation will guide you through setting up and using the library to create stunning LED animations.

## Features

- Control a NeoPixel LED strip.
- Create and manage multiple shooting star animations.
- Apply a twinkle effect to the LED strip.
- Adjustable parameters for animation control.

## Setup

### Prerequisites

- Arduino IDE installed on your computer.
- Adafruit NeoPixel library installed. You can install it via the Arduino Library Manager.

### Installation

1. Download the `LEDController` library.
2. Place the `LEDController` folder in the `libraries` directory of your Arduino sketchbook.

## Usage

### Initialization

1. **Include the library in your sketch:**

    ```cpp
    #include "LEDController.h"
    ```

2. **Define the number of LEDs and the type of NeoPixel:**

    ```cpp
    #define NUM_LEDS 50
    #define TYPE NEO_GRB + NEO_KHZ800
    ```

3. **Create an instance of the `LEDController` class:**

    ```cpp
    LEDController ledController(NUM_LEDS, 5, TYPE);
    ```

4. **Initialize the LED strip in the `setup` function:**

    ```cpp
    void setup() {
      Serial.begin(115200);
      ledController.begin();
      ledController.startTwinkle(50, 30, 1);  // max brightness of 50, update interval of 30ms, and fade amount of 1
      ledController.setTwinkleChance(500);    // Set twinkle chance
      randomSeed(analogRead(0));              // Initialize random number generator
    }
    ```

### Creating Shooting Stars

To create a shooting star animation, use the `startShootingStar` method:

```cpp
void loop() {
  uint8_t rgb[3] = { random(255), random(255), random(255) };

  if (random(300) == 0) {
    ledController.startShootingStar(0, 9, rgb, 255, 17, 50); // Custom interval
  }

  if (random(300) == 0) {
    ledController.startShootingStar(20, 29, rgb, 255, 17, 50);
  }

  if (random(300) == 0) {
    ledController.startShootingStar(30, 39, rgb, 255, 17, 50);
  }
  
  ledController.update();
  delay(15);
}
```

- **Parameters:**
  - `startLED`: The starting LED index.
  - `endLED`: The ending LED index.
  - `rgb[]`: An array containing the RGB color values.
  - `maxBrightness`: The maximum brightness of the shooting star.
  - `fadeSteps`: The steps to fade the LEDs.
  - `updateInterval`: The interval at which the shooting star updates.

### Configuring the Twinkle Effect

To start and configure the twinkle effect, use the `startTwinkle` method:

```cpp
ledController.startTwinkle(50, 30, 1);  // max brightness of 50, update interval of 30ms, and fade amount of 1
```

- **Parameters:**
  - `maxBrightness`: The maximum brightness of the twinkle.
  - `updateInterval`: The update interval in milliseconds.
  - `fadeAmount`: The amount by which the brightness should fade.

To adjust the chance of the twinkle effect occurring, use the `setTwinkleChance` method:

```cpp
ledController.setTwinkleChance(500);  // Set twinkle chance
```

### Updating the LED Strip

The `update` method should be called in the `loop` function to update the state of all animations:

```cpp
void loop() {
  ledController.update();
  delay(15);
}
```

## API Reference

### LEDController Class

- **`LEDController(uint16_t numLeds, uint8_t pin, neoPixelType type)`**
  - Constructor to initialize the LEDController.

- **`void begin()`**
  - Initializes the LED strip.

- **`void startShootingStar(uint16_t startLED, uint16_t endLED, uint8_t rgb[], uint8_t maxBrightness, uint8_t fadeSteps, uint16_t updateInterval)`**
  - Starts a new shooting star animation.
  - **Parameters:**
    - `startLED`: The starting LED index.
    - `endLED`: The ending LED index.
    - `rgb[]`: An array containing the RGB color values.
    - `maxBrightness`: The maximum brightness of the shooting star.
    - `fadeSteps`: The steps to fade the LEDs.
    - `updateInterval`: The interval at which the shooting star updates.

- **`void update()`**
  - Updates the state of all animations.

- **`bool isLEDAnimating()`**
  - Checks if any LED is currently animating.

- **`void lightUp(uint16_t startLED, uint16_t endLED, uint8_t grb[])`**
  - Lights up a specified range of LEDs with a given color.
  - **Parameters:**
    - `startLED`: The starting LED index.
    - `endLED`: The ending LED index.
    - `grb[]`: An array containing the GRB color values.

- **`void setFadeSteps(uint8_t fadeSteps)`**
  - Sets the fade steps for all shooting stars.
  - **Parameters:**
    - `fadeSteps`: The steps to fade the LEDs.

- **`void startTwinkle(uint8_t maxBrightness, uint16_t updateInterval, uint8_t fadeAmount)`**
  - Starts the twinkle effect with specified parameters.
  - **Parameters:**
    - `maxBrightness`: The maximum brightness of the twinkle.
    - `updateInterval`: The update interval in milliseconds.
    - `fadeAmount`: The amount by which the brightness should fade.

- **`void updateTwinkle()`**
  - Updates the state of the twinkle effect for each LED.

- **`void setTwinkleChance(uint16_t chance)`**
  - Sets the chance of the twinkle effect occurring.
  - **Parameters:**
    - `chance`: The chance of the twinkle effect occurring.

- **`bool isWithinShootingStarRange(uint16_t ledIndex)`**
  - Checks if a given LED index is within the range of an active shooting star.
  - **Parameters:**
    - `ledIndex`: The index of the LED to check.

- **`uint32_t adjustBrightness(uint32_t color, uint8_t brightness)`**
  - Adjusts the brightness of a given color.
  - **Parameters:**
    - `color`: The color to adjust.
    - `brightness`: The brightness level.

## Custom Effects

To create custom effects, you can combine the existing methods or create new ones based on your requirements. Here are some ideas:

### Blinking Effect

To create a simple blinking effect, you can alternate between turning the LEDs on and off:

```cpp
void loop() {
  ledController.lightUp(0, NUM_LEDS, {255, 0, 0});  // Red color
  delay(500);
  ledController.lightUp(0, NUM_LEDS, {0, 0, 0});    // Turn off
  delay(500);
}
```

### Custom Pattern

You can create a custom pattern by lighting up specific LEDs in a sequence:

```cpp
void loop() {
  ledController.lightUp(0, 10, {255, 0, 0});  // Red color
  delay(200);
  ledController.lightUp(10, 20, {0, 255, 0}); // Green color
  delay(200);
  ledController.lightUp(20, 30, {0, 0, 255}); // Blue color
  delay(200);
  ledController.lightUp(30, 40, {255, 255, 0}); // Yellow color
  delay(200);
}
```

## License

This library is licensed under the MIT License. See the `LICENSE` file for more details.

## Support

For support and questions, please create an issue on the GitHub repository.

---
