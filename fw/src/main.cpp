#include <Arduino.h>
#include <Keyboard.h>

#include "config/config.h"
#include "hal/matrix.h"
#include "hal/encoder.h"
#include "hal/leds.h"
#include "hal/display.h"
#include "hid/hid.h"
#include "storage/storage.h"
#include "ui/ui.h"

static Matrix    matrix;
static Encoder   encoder0(PIN_ENC0_CLK, PIN_ENC0_DT, PIN_ENC0_SW);  // navigation
static Encoder   encoder1(PIN_ENC1_CLK, PIN_ENC1_DT, PIN_ENC1_SW);  // media
static Leds      leds;
static Display   display;
static HID       hid;
static Storage   storage;
static UI        ui;
static AppConfig cfg;

static uint32_t lastUiTick   = 0;
static uint32_t lastIdleTick = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("[main] booting...");

    storage.begin();
    storage.load(cfg);

    matrix.begin();
    encoder0.begin();
    encoder1.begin();

    leds.begin();
    leds.setBrightness(cfg.brightness);
    leds.setLayerColor(cfg.layers[cfg.activeLayer].color);
    leds.show();

    display.begin();
    ui.begin(&cfg);

    hid.begin();

    Serial.println("[main] ready");
}

void loop() {
    uint32_t now = millis();

    // ── LVGL tick ─────────────────────────────────────────────────────────────
    if (now - lastUiTick >= UI_TICK_MS) {
        lastUiTick = now;
        display.tick();
    }
    display.update();

    // ── Key matrix ────────────────────────────────────────────────────────────
    matrix.scan();
    const Layer& layer = cfg.layers[cfg.activeLayer];
    for (uint8_t i = 0; i < MATRIX_KEY_COUNT; i++) {
        if (matrix.justPressed(i)) {
            uint8_t row = i / MATRIX_COLS, col = i % MATRIX_COLS;
            Serial.print("[key] PRESS r"); Serial.print(row);
            Serial.print(" c"); Serial.print(col);
            Serial.print("  type="); Serial.print(layer.keys[i].type);
            Serial.print(" code=0x"); Serial.println(layer.keys[i].code, HEX);
            hid.pressKey(layer.keys[i]);
            leds.flash(i, 255, 255, 255, 50);
            lastIdleTick = now;
        }
        if (matrix.justReleased(i)) {
            Serial.print("[key] RELEASE r"); Serial.print(i / MATRIX_COLS);
            Serial.print(" c"); Serial.println(i % MATRIX_COLS);
            hid.releaseKey(layer.keys[i]);
        }
    }

    // ── Encoder 0 — navigation UI ─────────────────────────────────────────────
    encoder0.update();
    int8_t delta0 = encoder0.getDelta();
    if (delta0 != 0) {
        Serial.print("[enc0] delta="); Serial.println(delta0);
        ui.onEncoderDelta(delta0);
        lastIdleTick = now;
    }
    if (encoder0.buttonPressed()) {
        Serial.println("[enc0] PRESS");
        ui.onEncoderPress();
        lastIdleTick = now;
    }

    // ── Encoder 1 — volume ────────────────────────────────────────────────────
    encoder1.update();
    int8_t delta1 = encoder1.getDelta();
    if (delta1 != 0) {
        uint16_t code = (delta1 > 0) ? 0xE9 : 0xEA;  // vol+ / vol-
        uint8_t steps = abs(delta1);
        for (uint8_t s = 0; s < steps; s++) {
            Keyboard.consumerPress((uint16_t)code);
            Keyboard.consumerRelease();
        }
        Serial.print("[vol] "); Serial.println(delta1 > 0 ? "+" : "-");
        lastIdleTick = now;
    }
    if (encoder1.buttonPressed()) {
        Keyboard.consumerPress((uint16_t)0xE2);  // mute
        Keyboard.consumerRelease();
        Serial.println("[vol] MUTE");
        lastIdleTick = now;
    }

    // ── UI update ─────────────────────────────────────────────────────────────
    ui.update();

    // ── Screen timeout ────────────────────────────────────────────────────────
    if (now - lastIdleTick > SCREEN_TIMEOUT_MS)
        display.setBacklight(30);
    else
        display.setBacklight(255);
}
