# Makerpad

A fully open-source macropad built from scratch — custom PCB, 3D-printed enclosure, and embedded firmware. 20 programmable keys, two rotary encoders, a 2" color display driven by LVGL, and 22 addressable RGB LEDs, all running on an RP2040.

Everything is designed to be reproducible: order the PCB, print the case, flash the firmware, and configure your keymap via a JSON file.

---

## What's inside

| Directory | Description |
|---|---|
| [`ecad/`](ecad/README.md) | KiCad schematic + PCB, BOM, custom footprints |
| [`mcad/`](mcad/README.md) | STEP files for the enclosure |
| [`fw/`](fw/README.md) | PlatformIO firmware (C++, RP2040) |
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

## Roadmap

- [x] Layer switching via encoder
- [x] Volume control with speaker/mic toggle
- [ ] Macro sequence support
- [ ] Per-key LED color from config
- [ ] OTA config update via USB mass storage

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for the commit convention, scope definitions, and contribution guidelines.
