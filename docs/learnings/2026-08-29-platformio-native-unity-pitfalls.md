# PlatformIO native Unity env pitfalls

Session learnings from adding `test/test_native/` (20 tests) as a host-side
Unity env on 2026-08-29.

## Unity 2.x API

- PlatformIO `test_framework = unity` pulls Unity 2.6.1. Assertions are
  `TEST_ASSERT_*` (`TEST_ASSERT_EQUAL`, `TEST_ASSERT_TRUE`,
  `TEST_ASSERT_EQUAL_FLOAT`). The older two-argument `TEST_ASSERT(condition,
  message)` form does not exist in this version.
- `TEST_ASSERT_FLOAT` is not an assertion in Unity 2.x; use
  `TEST_ASSERT_EQUAL_FLOAT(expected, actual)`.

## One program per test dir

- All files in a test dir link into a single host binary, so exactly one
  `main()` per dir. `test_main.cpp` declares the suites (`extern
  void test_*(void)`) and runs them via `RUN_TEST`.
- Suite files define plain `void test_...` functions; no per-file main.

## test_build_src = off

- `[env:native]` sets `test_build_src = off`: src/ is not compiled as a build
  target. Tests `#include` the pure, Arduino-free module `.cpp` directly
  (e.g. `#include "settings.cpp"`), pulling the code under test into the test
  binary without the Arduino HAL.
- Works only for host-compilable modules (no Arduino.h, no ESP32 HAL).
  Hardware-facing code stays covered by firmware build + boot capture.

## Bare `pio run` gotcha

- A bare `pio run` targets every env in platformio.ini, including the native
  test env, which tries to compile src/ for the host and fails on
  Arduino includes (verified 2026-08-29). Always pass explicit envs:
  `pio run -e esp32-c3-devkitm-1 -e esp32dev`.
- Also documented in the platformio.ini header.

## Inherited `[env]` keys

- Common-section keys (`platform`, `framework`, `lib_deps`,
  `board_build.partitions`, `extra_scripts`) are inherited by `[env:native]`
  and must be cleared or overridden (`framework =`, `lib_deps =`,
  `extra_scripts =`), or the host build tries to fetch ESP32 packages.
