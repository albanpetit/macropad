#include <Arduino.h>
#include <Keyboard.h>
#include <lvgl.h>

#include "config/config.h"
#include "hal/matrix.h"
#include "hal/encoder.h"
#include "hal/leds.h"
#include "hal/display.h"
#include "hid/hid.h"
#include "storage/storage.h"
#include "ui/ui.h"

static Matrix    matrix;
static Encoder   encoder0(PIN_ENC0_CLK, PIN_ENC0_DT, PIN_ENC0_SW);
static Encoder   encoder1(PIN_ENC1_CLK, PIN_ENC1_DT, PIN_ENC1_SW);
static Leds      leds;
static Display   display;
static HID       hid;
static Storage   storage;
static UI        ui;
static AppConfig cfg;

static uint32_t lastUiTick   = 0;
static uint32_t lastIdleTick = 0;

enum class Enc1Mode { Speaker, Mic };
static Enc1Mode enc1Mode      = Enc1Mode::Speaker;
static bool     enc1LongFired = false;

void setup() {
    Serial.begin(115200);
    delay(2000);  // délai fixe — laisse USB composite HID+CDC s'établir
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

    // HID en premier : Keyboard.begin() cause une ré-énumération USB
    // qui détruit l'état SPI si elle arrive après display.begin()
    hid.begin();
    delay(2000);  // attendre la fin de la ré-énumération avant d'init le SPI

    display.begin();
    ui.begin(&cfg);

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

    // ── Encoder 1 — speaker / mic (long press = toggle mode) ─────────────────
    encoder1.update();
    if (encoder1.buttonHeld(600)) {
        if (!enc1LongFired) {
            enc1LongFired = true;
            enc1Mode = (enc1Mode == Enc1Mode::Speaker) ? Enc1Mode::Mic : Enc1Mode::Speaker;
            bool isMic = (enc1Mode == Enc1Mode::Mic);
            Serial.print("[enc1] mode="); Serial.println(isMic ? "MIC" : "SPEAKER");
            leds.setAll(isMic ? 255 : 0, isMic ? 80 : 100, isMic ? 0 : 255);
            leds.show();
            lastIdleTick = now;
        }
    } else {
        enc1LongFired = false;
    }

    if (encoder1.buttonPressed() && !enc1LongFired) {
        uint16_t code = (enc1Mode == Enc1Mode::Speaker) ? 0xE2 : 0xCF;
        Keyboard.consumerPress((uint16_t)code);
        Keyboard.consumerRelease();
        Serial.println(enc1Mode == Enc1Mode::Speaker ? "[enc1] MUTE SPK" : "[enc1] MUTE MIC");
        lastIdleTick = now;
    }

    int8_t delta1 = encoder1.getDelta();
    if (delta1 != 0) {
        uint16_t code = (delta1 > 0) ? 0xE9 : 0xEA;
        uint8_t steps = abs(delta1);
        for (uint8_t s = 0; s < steps; s++) {
            Keyboard.consumerPress((uint16_t)code);
            Keyboard.consumerRelease();
        }
        Serial.print(enc1Mode == Enc1Mode::Mic ? "[mic] " : "[spk] ");
        Serial.println(delta1 > 0 ? "+" : "-");
        lastIdleTick = now;
    }

    // ── UI update ─────────────────────────────────────────────────────────────
    ui.update();

    // ── Sync LEDs quand le layer change via UI ────────────────────────────────
    if (ui.layerChanged()) {
        leds.setLayerColor(cfg.layers[cfg.activeLayer].color);
        leds.show();
        storage.save(cfg);
    }

    // ── Screen timeout ────────────────────────────────────────────────────────
    if (now - lastIdleTick > SCREEN_TIMEOUT_MS)
        display.setBacklight(30);
    else
        display.setBacklight(255);
}
