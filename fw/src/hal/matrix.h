#pragma once
#include <Arduino.h>
#include "../config/config.h"
#include "../config/keymap.h"

class Matrix {
public:
    void begin();
    void scan();

    bool isPressed(uint8_t row, uint8_t col) const;
    bool justPressed(uint8_t row, uint8_t col) const;
    bool justReleased(uint8_t row, uint8_t col) const;

    // Flat index: row * MATRIX_COLS + col
    bool isPressed(uint8_t idx) const    { return isPressed(idx / MATRIX_COLS, idx % MATRIX_COLS); }
    bool justPressed(uint8_t idx) const  { return justPressed(idx / MATRIX_COLS, idx % MATRIX_COLS); }
    bool justReleased(uint8_t idx) const { return justReleased(idx / MATRIX_COLS, idx % MATRIX_COLS); }

private:
    uint32_t _current  = 0;
    uint32_t _previous = 0;
    uint32_t _debounce = 0;
    uint32_t _lastScan = 0;

    uint32_t _readRaw();
};
