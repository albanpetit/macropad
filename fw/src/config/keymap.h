#pragma once
#include <stdint.h>
#include "config.h"

// Key action types
enum KeyType : uint8_t {
    KEY_NONE     = 0,
    KEY_HID      = 1,  // Standard HID keycode
    KEY_CONSUMER = 2,  // Consumer control (media, volume)
    KEY_LAYER    = 3,  // Switch to layer N  (planned)
    KEY_MACRO    = 4,  // Macro sequence     (planned)
};

struct KeyDef {
    KeyType type;
    uint16_t code;
};

struct Layer {
    char     name[MAX_LAYER_NAME_LEN];
    uint8_t  color[3];   // RGB for LED feedback
    KeyDef   keys[MATRIX_KEY_COUNT];
};

// ─── Default keymap ───────────────────────────────────────────────────────────
// Layout (row-major, col 0..3 left to right, row 0..4 top to bottom):
//
//  [0,0] [0,1] [0,2] [0,3]
//  [1,0] [1,1] [1,2] [1,3]
//  [2,0] [2,1] [2,2] [2,3]
//  [3,0] [3,1] [3,2] [3,3]
//  [4,0] [4,1] [4,2] [4,3]
//
// HID keycodes: https://usb.org/sites/default/files/hut1_4.pdf p.53
// Consumer codes: p.79 (0xE0=vol-, 0xE9=vol+, 0xCD=play/pause, etc.)

static const Layer DEFAULT_LAYER = {
    "Default",
    { 0, 100, 255 },  // blue
    {
        // Row 0
        { KEY_HID,      0x04 },  // A
        { KEY_HID,      0x05 },  // B
        { KEY_HID,      0x06 },  // C
        { KEY_HID,      0x07 },  // D
        // Row 1
        { KEY_HID,      0x08 },  // E
        { KEY_HID,      0x09 },  // F
        { KEY_HID,      0x0A },  // G
        { KEY_HID,      0x0B },  // H
        // Row 2
        { KEY_CONSUMER, 0xCD },  // Play/Pause
        { KEY_CONSUMER, 0xE9 },  // Volume+
        { KEY_CONSUMER, 0xEA },  // Volume-
        { KEY_CONSUMER, 0xB5 },  // Next track
        // Row 3
        { KEY_HID,      0x28 },  // Enter
        { KEY_HID,      0x2C },  // Space
        { KEY_HID,      0x2B },  // Tab
        { KEY_HID,      0x29 },  // Escape
        // Row 4
        { KEY_NONE,     0x00 },
        { KEY_NONE,     0x00 },
        { KEY_NONE,     0x00 },
        { KEY_NONE,     0x00 },
    }
};
