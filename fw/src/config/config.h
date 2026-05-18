#pragma once

// ─── Display ─────────────────────────────────────────────────────────────────
#define PIN_LCD_CS   9
#define PIN_LCD_SCLK 10
#define PIN_LCD_MOSI 11
#define PIN_LCD_BL   12
#define PIN_LCD_RST  13
#define PIN_LCD_DC   14
#define LCD_WIDTH    320
#define LCD_HEIGHT   240
#define LCD_ROTATION 1   // 0=portrait, 1=landscape

// ─── LEDs ─────────────────────────────────────────────────────────────────────
#define PIN_LED_DATA 3
#define LED_COUNT    22
#define LED_DEFAULT_BRIGHTNESS 80

// ─── Key matrix ───────────────────────────────────────────────────────────────
#define MATRIX_COLS 4
#define MATRIX_ROWS 5
#define MATRIX_KEY_COUNT (MATRIX_COLS * MATRIX_ROWS)
#define MATRIX_DEBOUNCE_MS 10

static const uint8_t MATRIX_COL_PINS[MATRIX_COLS] = { 21, 20, 19, 18 };
static const uint8_t MATRIX_ROW_PINS[MATRIX_ROWS] = { 26, 25, 24, 23, 22 };

// ─── Encoder ──────────────────────────────────────────────────────────────────
// TODO: verify these pins against the hardware
#define PIN_ENC_CLK 16
#define PIN_ENC_DT  17
#define PIN_ENC_SW  2

// ─── Potentiometers (ADC) ─────────────────────────────────────────────────────
#define PIN_POT_1 27  // ADC1
#define PIN_POT_2 28  // ADC2
#define PIN_POT_3 29  // ADC3

// ─── Storage ──────────────────────────────────────────────────────────────────
#define CONFIG_FILE "/config.json"
#define MAX_LAYERS  8
#define MAX_LAYER_NAME_LEN 16

// ─── UI ───────────────────────────────────────────────────────────────────────
#define UI_TICK_MS 5
#define SCREEN_TIMEOUT_MS 30000  // dim backlight after 30s idle
