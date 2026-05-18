#include "hid.h"

void HID::begin() {
    Keyboard.begin();
}

void HID::pressKey(const KeyDef& key) {
    switch (key.type) {
        case KEY_HID:
            Keyboard.press((uint8_t)key.code);
            break;
        case KEY_CONSUMER:
            Keyboard.consumerPress((uint16_t)key.code);
            break;
        default:
            break;
    }
}

void HID::releaseKey(const KeyDef& key) {
    switch (key.type) {
        case KEY_HID:
            Keyboard.release((uint8_t)key.code);
            break;
        case KEY_CONSUMER:
            Keyboard.consumerRelease();
            break;
        default:
            break;
    }
}

void HID::releaseAll() {
    Keyboard.releaseAll();
    Keyboard.consumerRelease();
}
