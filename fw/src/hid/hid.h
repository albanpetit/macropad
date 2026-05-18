#pragma once
#include <Arduino.h>
#include <Keyboard.h>
#include "../config/keymap.h"

class HID {
public:
    void begin();
    void pressKey(const KeyDef& key);
    void releaseKey(const KeyDef& key);
    void releaseAll();
};
