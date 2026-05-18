# Macropad

Custom RP2040-based macropad with RGB LEDs, LVGL display, rotary encoder and configurable key matrix.

## Hardware

| Component | Part | Notes |
|---|---|---|
| MCU | RP2040 (LQFN-56) | 133 MHz, 264 KB RAM |
| Flash | W25Q128JVP | 16 MB SPI flash |
| Display | HS20HS072RX | 2" 320×240 ST7789 SPI |
| LEDs | WS2812B-2020 × 22 | Addressable RGB, 5V |
| LED buffer | SN74LV1T34DBV | 3.3V → 5V level shifter |
| Key matrix | 4 cols × 5 rows | 20 keys |
| Encoder | PEC12R-4225F-S0024 | 24 PPR + push button |
| LDO | AP2112K-3.3V | 3.3V regulator |
| Crystal | ABM8 12 MHz | 10 pF load |

### GPIO Map

| GPIO | Signal | Direction |
|---|---|---|
| GPIO3 | LED DIN (via SN74LV1T34) | OUT |
| GPIO9 | LCD CS | OUT |
| GPIO10 | LCD SCL (SPI) | OUT |
| GPIO11 | LCD SDA / MOSI (SPI) | OUT |
| GPIO12 | LCD BLK (backlight) | OUT |
| GPIO13 | LCD RES (reset) | OUT |
| GPIO14 | LCD DC | OUT |
| GPIO16 | Encoder CLK (A) | IN |
| GPIO17 | Encoder DT (B) | IN |
| GPIO2 | Encoder SW (button) | IN |
| GPIO18 | Switch col 4 | OUT |
| GPIO19 | Switch col 3 | OUT |
| GPIO20 | Switch col 2 | OUT |
| GPIO21 | Switch col 1 | OUT |
| GPIO22 | Switch row 5 | IN |
| GPIO23 | Switch row 4 | IN |
| GPIO24 | Switch row 3 | IN |
| GPIO25 | Switch row 2 | IN |
| GPIO26 | Switch row 1 | IN |
| GPIO27 | ADC pot 1 | IN (ADC) |
| GPIO28 | ADC pot 2 | IN (ADC) |
| GPIO29 | ADC pot 3 | IN (ADC) |

> **Note:** GPIO16, GPIO17, GPIO2 are inferred for the encoder — verify against the schematic before use.

## Architecture

```
fw/
├── src/
│   ├── main.cpp              # Entry point, task scheduler
│   ├── lv_conf.h             # LVGL compile-time config
│   ├── config/
│   │   ├── config.h          # Pin definitions, build constants
│   │   └── keymap.h          # Default keymap (layers)
│   ├── hal/                  # Hardware Abstraction Layer
│   │   ├── matrix.h/.cpp     # Key matrix scanning
│   │   ├── encoder.h/.cpp    # Rotary encoder
│   │   ├── leds.h/.cpp       # WS2812B via NeoPixel
│   │   └── display.h/.cpp    # ST7789 + LVGL flush bridge
│   ├── hid/
│   │   └── hid.h/.cpp        # USB HID (keyboard + consumer)
│   ├── storage/
│   │   └── storage.h/.cpp    # LittleFS + ArduinoJson config
│   └── ui/
│       ├── ui.h/.cpp         # LVGL init, screen manager
│       └── screens/
│           ├── home.h/.cpp   # Home screen (layer status, LEDs)
│           └── settings.h/.cpp # Key binding editor
└── data/
    └── config.json           # Default config (flashed to LittleFS)
```

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/)
- TiGard or PicoProbe for SWD flashing

### Flash firmware

```bash
# Via SWD (TiGard)
pio run -e pico_tigard -t upload

# Via USB BOOTSEL
pio run -e pico -t upload
```

### Flash default config to LittleFS

```bash
pio run -e pico_tigard -t uploadfs
```

### Monitor serial

```bash
pio device monitor -e pico_tigard
```

## Key Configuration

Keys are stored in `data/config.json` and loaded from LittleFS at boot. Each layer holds 20 key definitions (row-major, 4 cols × 5 rows).

```json
{
  "brightness": 128,
  "active_layer": 0,
  "layers": [
    {
      "name": "Default",
      "color": [0, 0, 255],
      "keys": [
        { "type": "key",      "code": 4   },
        { "type": "consumer", "code": 233 },
        { "type": "key",      "code": 0   }
      ]
    }
  ]
}
```

### Key types

| Type | Description |
|---|---|
| `key` | Standard HID keycode |
| `consumer` | Consumer control (media, volume) |
| `macro` | Sequence of keycodes *(planned)* |
| `layer` | Layer switch *(planned)* |

## Contributing

1. Fork the repo and create a feature branch
2. Pin definitions live only in `src/config/config.h`
3. All HID logic is isolated in `src/hid/` — no direct hardware calls
4. New UI screens go in `src/ui/screens/`
5. Open a PR with a clear description following the commit convention below

### Roadmap

- [ ] Confirm encoder GPIO pins on hardware
- [ ] Macro sequence support
- [ ] Layer switching via encoder long-press
- [ ] Potentiometer → volume / scroll mapping
- [ ] Per-key LED color in config
- [ ] OTA config update via USB mass storage

---

## Commit Convention

**type**(**scope**): **description**

### Types

| Type | Usage |
|---|---|
| `feat` | New feature |
| `fix` | Bug fix |
| `style` | Formatting, renaming — no logic change |
| `refactor` | Refactoring without feature change |
| `perf` | Performance improvements |
| `test` | Adding or updating tests |
| `build` | Build system or dependency changes |
| `ci` | CI/CD changes |
| `chore` | Other non-product changes |

### Scopes

| Scope | Area |
|---|---|
| `ecad` | Schematics, PCB, components |
| `mcad` | 3D modeling, enclosure |
| `fw` | Embedded firmware |
| `sw` | PC / web software |
| `doc` | Documentation |
| `repo` | Repo structure, scripts |

### Examples

```
feat(fw): add layer switching via encoder long-press
fix(fw): fix key debounce issue on column 3
feat(ecad): add USB-C connector to schematic
docs(repo): update GPIO map in README
```

Use English or French consistently within a PR.
