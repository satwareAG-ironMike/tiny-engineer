# DOX framework

- DOX is highly performant AGENTS.md hierarchy installed here
- Agent must follow DOX instructions across any edits

## Core Contract

- AGENTS.md files are binding work contracts for their subtrees
- Work products, source materials, instructions, records, assets, and durable docs must stay understandable from the nearest applicable AGENTS.md plus every parent AGENTS.md above it

## Read Before Editing

1. Read the root AGENTS.md
2. Identify every file or folder you expect to touch
3. Walk from the repository root to each target path
4. Read every AGENTS.md found along each route
5. If a parent AGENTS.md lists a child AGENTS.md whose scope contains the path, read that child and continue from there
6. Use the nearest AGENTS.md as the local contract and parent docs for repo-wide rules
7. If docs conflict, the closer doc controls local work details, but no child doc may weaken DOX

Do not rely on memory. Re-read the applicable DOX chain in the current session before editing.

## Update After Editing

Every meaningful change requires a DOX pass before the task is done.

Update the closest owning AGENTS.md when a change affects:

- purpose, scope, ownership, or responsibilities
- durable structure, contracts, workflows, or operating rules
- required inputs, outputs, permissions, constraints, side effects, or artifacts
- user preferences about behavior, communication, process, organization, or quality
- AGENTS.md creation, deletion, move, rename, or index contents

Update parent docs when parent-level structure, ownership, workflow, or child index changes. Update child docs when parent changes alter local rules. Remove stale or contradictory text immediately. Small edits that do not change behavior or contracts may leave docs unchanged, but the DOX pass still must happen.

## Hierarchy

- Root AGENTS.md is the DOX rail: project-wide instructions, global preferences, durable workflow rules, and the top-level Child DOX Index
- Child AGENTS.md files own domain-specific instructions and their own Child DOX Index
- Each parent explains what its direct children cover and what stays owned by the parent
- The closer a doc is to the work, the more specific and practical it must be

## Child Doc Shape

- Create a child AGENTS.md when a folder becomes a durable boundary with its own purpose, rules, responsibilities, workflow, materials, or quality standards
- Work Guidance must reflect the current standards of the project or user instructions; if there are no specific standards or instructions yet, leave it empty
- Verification must reflect an existing check; if no verification framework exists yet, leave it empty and update it when one exists

Default section order:
- Purpose
- Ownership
- Local Contracts
- Work Guidance
- Verification
- Child DOX Index

## Style

- Keep docs concise, current, and operational
- Document stable contracts, not diary entries
- Put broad rules in parent docs and concrete details in child docs
- Prefer direct bullets with explicit names
- Do not duplicate rules across many files unless each scope needs a local version
- Delete stale notes instead of explaining history
- Trim obvious statements, repeated rules, misplaced detail, and warnings for risks that no longer exist

## Closeout

1. Re-check changed paths against the DOX chain
2. Update nearest owning docs and any affected parents or children
3. Refresh every affected Child DOX Index
4. Remove stale or contradictory text
5. Run existing verification when relevant
6. Report any docs intentionally left unchanged and why

## satware Engineering Values

Binding standards for all work in this repository (full satware AG harness rules apply on top).

| Value | Standard | State |
|-------|----------|-------|
| SDD | Work is specified before implementation: `specs/<feature>/spec.md` -> `plan.md` -> `tasks.md`; IPADP L1 via `specs/metadata.json` | Applied: `specs/metadata.json` (IPADP L1, upstream `jamro/tiny-engineer`); first per-feature spec pending |
| TDD | Test-first from spec (red -> green -> refactor). PlatformIO Unity: `[env:native]` (`platform = native`, `test_framework = unity`, `test_build_src = off`), tests in `test/test_native/` include the pure module `.cpp` under test directly | Applied: `pio test -e native` (20 tests: settings validators, eyes geometry/timing) |
| Baby Steps | Atomic commits, <200 LOC, one intent; commit format `<type>(<scope>): <msg>`; branches `feat/<scope>`, `fix/<scope>`, `docs/<scope>` | Applied (multi-arch port 2026-08-29) |
| Quality gate | `pio run -e esp32-c3-devkitm-1 -e esp32dev` green + `pio test -e native` green + hardware boot capture for hardware-facing changes | Applied (build + tests + boot capture) |
| Test determinism | Hermetic tests; injected clock for time logic (`now` parameters, no wall-clock); negative control for every fix commit (flip an assertion, watch it fail, revert) | Applied (native suite) |
| Multi-arch contract | Chip-specific code goes inside `CONFIG_IDF_TARGET_ESP32C3` guards (`include/pins.h`, `src/hardware/rgb.cpp`, `src/hardware/chip_temp.cpp`); all envs must build before commit | Applied |
| Trust boundary | All REST API inputs validated (`src/http/settings_handlers.cpp`); no silent defaults on malformed input | Applied |
| Punctuation | Em dash / en dash banned in all outputs; hyphen only | Applied |
| Never simplify away | Keep: input validation, NVS data safety, explicit user requests; never cut | Applied |

## Project Contract

Firmware for the tiny-engineer robot: ESP32 + PCA9685 servos + SSD1306 OLED + I2S WAV audio + WS2812 (C3 only). Platform: pioarduino platform-espressif32 (Arduino-ESP32 3.3.11), framework arduino.

| Fact | Value |
|------|-------|
| Envs | `esp32-c3-devkitm-1` (C3-Zero: I2C GP0/GP1, I2S GP2/GP3/GP4, WS2812 GP10, OE GP5) and `esp32dev` (classic D0WD: I2C 21/22, I2S 4/5/16, OE 17, no RGB LED) |
| Build | `pio run -e esp32-c3-devkitm-1 -e esp32dev` (bare `pio run` also targets the host test env) |
| Test | `pio test -e native` (host-side Unity 2.x, `TEST_ASSERT_*` API; one `main()` per test dir: `test_main.cpp` runs the suites) |
| Flash | `pio run -e <env> -t upload --upload-port /dev/ttyUSB0` (CH340 adapter) |
| Boot capture | pyserial from the PlatformIO venv with DTR/RTS toggle; `pio device monitor` needs an interactive TTY and is not agent-usable |
| Serial logging | Off by default (`SETTINGS_DEFAULT_SERIAL_LOG`, `src/settings.h`); flip to `true` for a debug build, revert before commit |
| First boot | `[E][Preferences.cpp:47] nvs_open failed: NOT_FOUND` is harmless: read-only open of empty NVS falls back to defaults |
| No PCA9685 | `initPca9685()` halts when 0x40 is absent; expected on a bare devkit |
| Audio assets | `assets/*.wav` is source of truth; `scripts/copy_assets.py` (pre-build, fails fast) syncs to `data/`; `scripts/upload_fs_after_upload.py` flashes `data/` after upload |

## User Preferences

When the user requests a durable behavior change, record it here or in the relevant child AGENTS.md

## Child DOX Index

No child AGENTS.md files exist yet. Durable boundaries to doxify when their local rules stabilize:

| Path | Scope |
|------|-------|
| `src/` | Firmware source: hardware drivers (pca9685, rgb, chip_temp), display, audio, animation state machine, http settings server |
| `include/` | Shared headers; `pins.h` owns the per-arch pin map |
| `docs/` | Descriptive docs: api, hardware, wiring, integration, settings, fork-state, learnings |
| `test/` | PlatformIO Unity tests (native env, `test/test_native/`: `test_main.cpp` runner + per-module suites) |
| `3d_models/` | Robot body CAD (`cad/`, `parts/`) |
| `specs/` | SDD specs per feature (`<feature>/spec.md` -> `plan.md` -> `tasks.md`); `metadata.json` owns IPADP L1 dependency metadata |
| `scripts/` | Build/ops scripts: `copy_assets.py`, `upload_fs_after_upload.py`, `gate-check-sod.sh` |

Root-owned files: `README.md`, `LICENSE`, `platformio.ini`, `partitions.csv`, `package.json`, `assets/` (audio source of truth), `lib/` (scaffold), `.github/workflows/` (CI: ESP autobuild), `.gitleaksignore` (secret-scan fingerprint suppressions). `data/` is a generated, gitignored payload (rebuilt by `scripts/copy_assets.py` pre-build).
