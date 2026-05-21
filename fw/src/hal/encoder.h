#pragma once
#include <Arduino.h>
#include "../config/config.h"

class Encoder {
public:
    Encoder(uint8_t pinClk, uint8_t pinDt, uint8_t pinSw);

    void begin();
    void update();   // call every loop for button debounce

    int8_t getDelta();
    bool   buttonPressed();
    bool   buttonHeld(uint32_t ms = 500);

private:
    uint8_t _pinClk, _pinDt, _pinSw;
    volatile int32_t _position = 0;
    int32_t  _lastPosition   = 0;
    uint8_t  _lastA          = HIGH;
    bool     _btnLastState   = HIGH;
    bool     _btnPressedFlag = false;
    uint32_t _btnPressTime   = 0;

    uint8_t _idx;

    // Up to 3 instances with dedicated ISRs
    static Encoder* _instances[3];
    static uint8_t  _count;

    static void _dispatchISR(uint8_t idx);

public:
    static void _isr0();
    static void _isr1();
    static void _isr2();
};
