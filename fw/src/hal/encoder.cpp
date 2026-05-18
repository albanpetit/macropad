#include "encoder.h"

Encoder* Encoder::_instance = nullptr;

void Encoder::begin() {
    _instance = this;
    pinMode(PIN_ENC_CLK, INPUT_PULLUP);
    pinMode(PIN_ENC_DT,  INPUT_PULLUP);
    pinMode(PIN_ENC_SW,  INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_CLK), _isrHandler, CHANGE);
}

void Encoder::update() {
    bool state = digitalRead(PIN_ENC_SW);
    if (_btnLastState == HIGH && state == LOW) {
        _btnPressTime    = millis();
        _btnPressedFlag  = true;
    }
    _btnLastState = state;
}

int8_t Encoder::getDelta() {
    noInterrupts();
    int32_t pos = _position;
    interrupts();
    int8_t delta = (int8_t)(pos - _lastPosition);
    _lastPosition = pos;
    return delta;
}

bool Encoder::buttonPressed() {
    if (_btnPressedFlag) {
        _btnPressedFlag = false;
        return true;
    }
    return false;
}

bool Encoder::buttonHeld(uint32_t ms) {
    return (_btnLastState == LOW) && (millis() - _btnPressTime >= ms);
}

void Encoder::_isrHandler() {
    if (!_instance) return;
    bool a = digitalRead(PIN_ENC_CLK);
    bool b = digitalRead(PIN_ENC_DT);
    if (a != _instance->_lastA) {
        _instance->_position += (a != b) ? 1 : -1;
        _instance->_lastA = a;
    }
}
