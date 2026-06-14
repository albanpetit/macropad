# Firmware

Built with [PlatformIO](https://platformio.org/) targeting the RP2040. Keys, layers and LED settings live in a JSON config file on LittleFS — no recompilation needed to remap keys.

---

## Getting started

```bash
pio run -e pico -t upload      # Flash firmware
pio run -e pico -t uploadfs    # Flash default config
pio device monitor -e pico     # Serial monitor (115200 baud)
```

---

## Key configuration

Config is stored in `data/config.json` and loaded from LittleFS at boot.

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

Each layer holds 20 key definitions (row-major, 4 cols × 5 rows).

| Key type | Description |
|---|---|
| `key` | Standard HID keycode |
| `consumer` | Consumer control (media, volume) |
| `macro` | Sequence of keycodes *(planned)* |

---

## Controls

| Control | Action |
|---|---|
| Encoder 0 rotate | Navigate layers |
| Encoder 0 press | Activate selected layer |
| Encoder 1 rotate | Volume up/down |
| Encoder 1 press | Mute |
| Encoder 1 long press (600 ms) | Toggle speaker / mic mode |

---

## Architecture

```
fw/src/
├── main.cpp              # Entry point, main loop
├── lv_conf.h             # LVGL compile-time config
├── config/
│   ├── config.h          # Pin definitions, build constants
│   └── keymap.h          # KeyDef, Layer types + default keymap
├── hal/
│   ├── matrix.h/.cpp     # Key matrix scanning (4×5, debounce)
│   ├── encoder.h/.cpp    # Rotary encoder via interrupt
│   ├── leds.h/.cpp       # WS2812B via NeoPixel
│   └── display.h/.cpp    # ST7789V2-HSD driver + LVGL flush bridge
├── hid/
│   └── hid.h/.cpp        # USB HID (keyboard + consumer control)
├── storage/
│   └── storage.h/.cpp    # LittleFS + ArduinoJson config persistence
└── ui/
    └── ui.h/.cpp         # LVGL single-screen layer list
```
