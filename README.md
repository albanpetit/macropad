# Macropad

Custom RP2040-based macropad with RGB LEDs, LVGL display, two rotary encoders and configurable key matrix.

## Hardware

| Component | Part | Notes |
|---|---|---|
| MCU | RP2040 (LQFN-56) | 133 MHz, 264 KB RAM |
| Flash | W25Q128JVP | 16 MB SPI flash |
| Display | HS20HS072RX | 2" 240×320 ST7789V2-HSD SPI (portrait) |
| LEDs | WS2812B-2020 × 22 | Addressable RGB, 5V |
| LED buffer | SN74LV1T34DBV | 3.3V → 5V level shifter |
| Key matrix | 4 cols × 5 rows | 20 keys |
| Encoders | PEC12R-4225F-S0024 × 2 | 24 PPR + push button |
| LDO | AP2112K-3.3V | 3.3V regulator |
| Crystal | ABM8 12 MHz | 10 pF load |

## Architecture

```
fw/
├── src/
│   ├── main.cpp              # Entry point, main loop
│   ├── lv_conf.h             # LVGL compile-time config
│   ├── config/
│   │   ├── config.h          # Pin definitions, build constants
│   │   └── keymap.h          # KeyDef, Layer types + default keymap
│   ├── hal/
│   │   ├── matrix.h/.cpp     # Key matrix scanning (4×5, debounce)
│   │   ├── encoder.h/.cpp    # Rotary encoder via interrupt
│   │   ├── leds.h/.cpp       # WS2812B via NeoPixel
│   │   └── display.h/.cpp    # ST7789V2-HSD driver + LVGL flush bridge
│   ├── hid/
│   │   └── hid.h/.cpp        # USB HID (keyboard + consumer control)
│   ├── storage/
│   │   └── storage.h/.cpp    # LittleFS + ArduinoJson config persistence
│   └── ui/
│       └── ui.h/.cpp         # LVGL single-screen layer list
└── data/
    └── config.json           # Default config (flashed to LittleFS)
```

## Getting Started

**Prerequisites:** [PlatformIO](https://platformio.org/)

```bash
pio run -e pico -t upload      # Flash firmware via USB
pio run -e pico -t uploadfs    # Flash default config to LittleFS
pio device monitor -e pico     # Serial monitor (115200 baud)
```

## Key Configuration

Keys are stored in `data/config.json` and loaded from LittleFS at boot. Each layer holds 20 key definitions (row-major, 4 cols × 5 rows).

```json
{
  "brightness": 80,
  "active_layer": 0,
  "layers": [
    {
      "name": "Default",
      "color": [0, 100, 255],
      "keys": [
        { "type": "key",      "code": 4   },
        { "type": "consumer", "code": 233 },
        { "type": "key",      "code": 0   }
      ]
    }
  ]
}
```

| Key type | Description |
|---|---|
| `key` | Standard HID keycode |
| `consumer` | Consumer control (media, volume) |
| `macro` | Sequence of keycodes *(planned)* |

## Controls

| Control | Action |
|---|---|
| Encoder 0 rotate | Navigate layer list |
| Encoder 0 press | Activate selected layer |
| Encoder 1 rotate | Volume up/down (speaker or mic) |
| Encoder 1 press | Mute active device |
| Encoder 1 long press (600ms) | Toggle speaker / mic mode |

## Roadmap

- [x] Layer switching via encoder 0
- [x] Encoder 1 speaker/mic volume control with mode toggle
- [ ] Macro sequence support
- [ ] Per-key LED color in config
- [ ] OTA config update via USB mass storage
