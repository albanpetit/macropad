# MCAD

3D models for the Makerpad enclosure, exported as STEP files.

---

## Files

| File | Description |
|---|---|
| `ecad.step` | Full PCB assembly export from KiCad |
| `kailh-switch.step` | Kailh low-profile key switch model |

---

## Usage

Both files are standard STEP (ISO 10303) and open in any CAD tool: FreeCAD, Fusion 360, SolidWorks, or directly in KiCad's 3D viewer.

To preview the full board assembly in KiCad:

1. Open `ecad/ecad.kicad_pcb`
2. Go to **View → 3D Viewer**
3. STEP models are resolved automatically from `ecad/3d-models/`

---

## Printing the enclosure

If you design a custom case around `ecad.step`:

- PCB dimensions and mounting hole positions are embedded in the STEP file
- Mounting holes: M2 (2.2 mm) and M3 (3.2 mm) — see `ecad/library.pretty/` for exact footprints
- Leave clearance for the USB-C connector, display FPC, and encoder shafts on the top face
