# Makerpad

A fully open-source macropad built from scratch — custom PCB, 3D-printed enclosure, and embedded firmware. 20 programmable keys, two rotary encoders, a 2" color display driven by LVGL, and 22 addressable RGB LEDs, all running on an RP2040.

Everything is designed to be reproducible: order the PCB, print the case, flash the firmware, and configure your keymap via a JSON file.

---

## What's inside

| Directory | Description |
|---|---|
| [`ecad/`](ecad/README.md) | KiCad schematic + PCB, BOM, custom footprints |
| [`mcad/`](mcad/README.md) | STEP files for the enclosure |
| `fw/` | PlatformIO firmware (C++, RP2040) |
| `doc/` | Datasheets, iBOM, schematic PDF |

---

## Hardware at a glance

| Component | Part |
|---|---|
| MCU | RP2040 (LQFN-56), 133 MHz, 264 KB RAM |
| Flash | W25Q128JVP — 16 MB SPI |
| Display | HS20HS072RX — 2" 240×320 ST7789V2-HSD SPI |
| RGB LEDs | WS2812B-2020 × 22 — addressable, 5V |
| Keys | 4 cols × 5 rows = 20 keys |
| Encoders | PEC12R-4225F-S0024 × 2 — 24 PPR + push |
| Connectivity | USB-C (HID keyboard + consumer control) |

---

## Firmware

The firmware is built with [PlatformIO](https://platformio.org/). Keys, layers and LED settings are stored in a JSON config file on LittleFS — no recompilation needed to remap keys.

```bash
pio run -e pico -t upload      # Flash firmware
pio run -e pico -t uploadfs    # Flash default config
pio device monitor -e pico     # Serial monitor (115200 baud)
```

### Layer config

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
        { "type": "consumer", "code": 233 }
      ]
    }
  ]
}
```

Key types: `key` (standard HID), `consumer` (media/volume), `macro` *(planned)*.

### Controls

| Control | Action |
|---|---|
| Encoder 0 rotate | Navigate layers |
| Encoder 0 press | Activate selected layer |
| Encoder 1 rotate | Volume up/down |
| Encoder 1 press | Mute |
| Encoder 1 long press (600 ms) | Toggle speaker / mic mode |

### Firmware architecture

```
fw/src/
├── main.cpp              # Entry point, main loop
├── config/               # Pin definitions, keymap types
├── hal/                  # Matrix, encoders, LEDs, display
├── hid/                  # USB HID (keyboard + consumer)
├── storage/              # LittleFS + JSON persistence
└── ui/                   # LVGL layer-list screen
```

---

## Roadmap

- [x] Layer switching via encoder
- [x] Volume control with speaker/mic toggle
- [ ] Macro sequence support
- [ ] Per-key LED color from config
- [ ] OTA config update via USB mass storage

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for the commit convention, scope definitions, and contribution guidelines.
