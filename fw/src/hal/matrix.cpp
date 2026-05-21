#include "matrix.h"

void Matrix::begin() {
    // Rows = OUTPUT strobe (drive LOW one at a time)
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        pinMode(MATRIX_ROW_PINS[r], OUTPUT);
        digitalWrite(MATRIX_ROW_PINS[r], HIGH);
    }
    // Cols = INPUT with pull-up (read LOW when key pressed)
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        pinMode(MATRIX_COL_PINS[c], INPUT_PULLUP);
    }
}

void Matrix::scan() {
    uint32_t now = millis();
    if (now - _lastScan < MATRIX_DEBOUNCE_MS) return;
    _lastScan = now;

    uint32_t raw = _readRaw();
    _previous = _current;
    _current  = raw;
}

bool Matrix::isPressed(uint8_t row, uint8_t col) const {
    return (_current >> (row * MATRIX_COLS + col)) & 1;
}

bool Matrix::justPressed(uint8_t row, uint8_t col) const {
    uint8_t idx = row * MATRIX_COLS + col;
    return ((_current >> idx) & 1) && !((_previous >> idx) & 1);
}

bool Matrix::justReleased(uint8_t row, uint8_t col) const {
    uint8_t idx = row * MATRIX_COLS + col;
    return !((_current >> idx) & 1) && ((_previous >> idx) & 1);
}

uint32_t Matrix::_readRaw() {
    uint32_t state = 0;
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        digitalWrite(MATRIX_ROW_PINS[r], LOW);
        delayMicroseconds(10);
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            if (!digitalRead(MATRIX_COL_PINS[c]))
                state |= (1u << (r * MATRIX_COLS + c));
        }
        digitalWrite(MATRIX_ROW_PINS[r], HIGH);
    }
    return state;
}
