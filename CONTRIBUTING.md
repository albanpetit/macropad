# Contributing

## Getting started

1. Fork the repo and create a feature branch from `main`
2. Pin definitions live only in `fw/src/config/config.h` — no hardcoded values elsewhere
3. All HID logic stays in `fw/src/hid/` — no direct `Keyboard.*` calls outside that module
4. Open a PR with a clear description and link any related issue

## Commit convention

```
type(scope): short description
```

### Types

| Type | Usage |
|---|---|
| `feat` | New feature |
| `fix` | Bug fix |
| `refactor` | Refactoring without behaviour change |
| `chore` | Non-product changes (deps, tooling, cleanup) |
| `perf` | Performance improvements |
| `style` | Formatting, renaming — no logic change |
| `build` | Build system or dependency changes |
| `ci` | CI/CD changes |
| `doc` | Documentation |

### Scopes

| Scope | Area |
|---|---|
| `ecad` | Schematics, PCB, components |
| `mcad` | 3D modelling, enclosure |
| `fw` | Embedded firmware |
| `doc` | Documentation |

### Examples

```
feat(fw): add per-key LED color from config
fix(fw): fix key debounce on column 3
refactor(ecad): reorganize footprint library
chore(fw): remove unused LVGL widgets
```

Use English or French consistently within a PR.
