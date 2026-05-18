#pragma once
#include <Adafruit_NeoPixel.h>
#include "../config/config.h"

class Leds {
public:
    void begin();
    void setBrightness(uint8_t brightness);
    void setAll(uint8_t r, uint8_t g, uint8_t b);
    void setKey(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
    void setLayerColor(const uint8_t color[3]);
    void flash(uint8_t idx, uint8_t r, uint8_t g, uint8_t b, uint16_t ms = 100);
    void show();
    void off();

private:
    Adafruit_NeoPixel _strip{ LED_COUNT, PIN_LED_DATA, NEO_GRB + NEO_KHZ800 };
};
