#include "encoder.h"

Encoder* Encoder::_instances[3] = { nullptr, nullptr, nullptr };
uint8_t  Encoder::_count = 0;

static void (*const _isrTable[3])() = {
    Encoder::_isr0,
    Encoder::_isr1,
    Encoder::_isr2,
};

Encoder::Encoder(uint8_t pinClk, uint8_t pinDt, uint8_t pinSw)
    : _pinClk(pinClk), _pinDt(pinDt), _pinSw(pinSw)
{
    _idx = _count < 3 ? _count++ : 0;
    _instances[_idx] = this;
}

void Encoder::begin() {
    pinMode(_pinClk, INPUT_PULLUP);
    pinMode(_pinDt,  INPUT_PULLUP);
    pinMode(_pinSw,  INPUT_PULLUP);
    _lastA = digitalRead(_pinClk);
    attachInterrupt(digitalPinToInterrupt(_pinClk), _isrTable[_idx], CHANGE);
}

void Encoder::update() {
    bool state = digitalRead(_pinSw);
    if (_btnLastState == HIGH && state == LOW) {
        _btnPressTime   = millis();
        _btnPressedFlag = true;
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

void Encoder::_dispatchISR(uint8_t idx) {
    Encoder* e = _instances[idx];
    if (!e) return;
    bool a = digitalRead(e->_pinClk);
    bool b = digitalRead(e->_pinDt);
    if (a != e->_lastA) {
        e->_position += (a != b) ? 1 : -1;
        e->_lastA = a;
    }
}

void Encoder::_isr0() { _dispatchISR(0); }
void Encoder::_isr1() { _dispatchISR(1); }
void Encoder::_isr2() { _dispatchISR(2); }
