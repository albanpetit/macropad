#include "leds.h"

void Leds::begin() {
    _strip.begin();
    _strip.setBrightness(LED_DEFAULT_BRIGHTNESS);
    _strip.clear();
    _strip.show();
}

void Leds::setBrightness(uint8_t b) {
    _strip.setBrightness(b);
}

void Leds::setAll(uint8_t r, uint8_t g, uint8_t b) {
    _strip.fill(_strip.Color(r, g, b));
}

void Leds::setKey(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (idx < LED_COUNT)
        _strip.setPixelColor(idx, _strip.Color(r, g, b));
}

void Leds::setLayerColor(const uint8_t color[3]) {
    setAll(color[0], color[1], color[2]);
}

void Leds::flash(uint8_t idx, uint8_t r, uint8_t g, uint8_t b, uint16_t ms) {
    uint32_t saved = _strip.getPixelColor(idx);
    setKey(idx, r, g, b);
    show();
    delay(ms);
    _strip.setPixelColor(idx, saved);
    show();
}

void Leds::show() {
    _strip.show();
}

void Leds::off() {
    _strip.clear();
    _strip.show();
}
