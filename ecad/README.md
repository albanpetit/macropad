# ECAD

PCB design for the Makerpad — schematic and layout done in [KiCad](https://www.kicad.org/).

---

## Files

| File / Directory | Description |
|---|---|
| `ecad.kicad_pro` | KiCad project root |
| `ecad.kicad_sch` | Top-level schematic |
| `switches.kicad_sch` | Key matrix sub-sheet |
| `leds.kicad_sch` | LED chain sub-sheet |
| `ecad.kicad_pcb` | PCB layout |
| `library.pretty/` | Custom footprints (RP2040, WS2812B-2020, encoders…) |
| `3d-models/` | STEP files for 3D preview |
| `ecad-backups/` | KiCad auto-backups |

Exported outputs live in `doc/`:

| Output | Description |
|---|---|
| `doc/bom.xlsx` / `bom.pdf` | Bill of materials |
| `doc/schematic.pdf` | Full schematic PDF |
| `doc/ibom/ibom.html` | Interactive BOM (open in browser) |
| `doc/ecad-export/` | Gerbers and drill files |
| `doc/datasheet/` | Datasheets for key components |

---

## Opening the project

```bash
kicad ecad.kicad_pro
```

Requires KiCad 8 or later. All symbols and footprints are embedded or resolved via `sym-lib-table` and `fp-lib-table` — no external library setup needed.

---

## Interactive BOM

Open `doc/ibom/ibom.html` in any browser. Click a component to highlight its position on the board — useful when soldering.

---

## Ordering the PCB

1. Use the Gerbers in `doc/ecad-export/` and upload them to your preferred fabricator (JLCPCB, PCBWay, OSHPark…).
2. Recommended specs: 2-layer, 1.6 mm FR4, HASL or ENIG finish, 0.2 mm min trace/space.
3. Cross-reference the BOM (`doc/bom.xlsx`) for part numbers and quantities.

---

## Custom footprints (`library.pretty/`)

| Footprint | Component |
|---|---|
| `QFN-56-1EP_7x7mm_P0.4mm` | RP2040 |
| `LED_WS2812B-2020_PLCC4_2.0x2.0mm` | WS2812B-2020 RGB LED |
| `Crystal_SMD_Abracon_ABM8G-4Pin_3.2x2.5mm` | 12 MHz crystal |
| `Hirose_FH12-12S-0.5SH` | 0.5 mm FPC connector (display) |
| `PinHeader_2x05_P1.27mm_Vertical_SMD` | SWD debug header |
