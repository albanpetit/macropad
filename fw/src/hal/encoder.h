#pragma once
#include <Arduino.h>
#include "../config/config.h"

class Encoder {
public:
    void begin();
    void update();

    int8_t getDelta();        // steps since last call (positive = CW)
    bool   buttonPressed();   // true once per press
    bool   buttonHeld(uint32_t ms = 500);

private:
    volatile int32_t _position = 0;
    int32_t _lastPosition      = 0;
    uint8_t _lastA             = HIGH;

    bool     _btnLastState   = HIGH;
    bool     _btnPressedFlag = false;
    uint32_t _btnPressTime   = 0;

    static void _isrHandler();
    static Encoder* _instance;
};
