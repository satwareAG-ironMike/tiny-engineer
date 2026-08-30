#pragma once

#include <cstdint>

// =====================================================
// PINOUT (multi-architecture)
// =====================================================
// One source tree, two targets. The active pin map is selected by the
// IDF target macro that PlatformIO sets per board:
//   CONFIG_IDF_TARGET_ESP32C3  -> Waveshare ESP32-C3-Zero (RISC-V)
//   anything else              -> classic ESP32 devkit (D0WD-V3)
//
// C3 pads: GP9 (USB D-) and GP11 (boot strapping) stay free.
// Classic pads: flash-reserved GPIOs (6-11, 12, 15) and strapping
// pins (0, 2, 15) stay free. UART0 stays free for serial logging.

#if defined(CONFIG_IDF_TARGET_ESP32C3)

// ---- ESP32-C3-Zero (Waveshare) ----

// I2C -> PCA9685 + OLED
constexpr int I2C_SDA = 0;   // GP0
constexpr int I2C_SCL = 1;   // GP1

// I2S -> MAX98357A
constexpr int I2S_BCLK = 2;  // GP2
constexpr int I2S_LRC  = 3;  // GP3
constexpr int I2S_DIN  = 4;  // GP4

// Onboard WS2812 RGB (GRB byte order)
constexpr bool RGB_LED_PRESENT = true;
constexpr int RGB_LED_PIN = 10;

// PCA9685 output enable (active LOW). Optional: wire OE -> GP5.
constexpr bool PCA9685_OE_WIRED = false;
constexpr int PCA9685_OE_PIN = 5;

#else

// ---- Classic ESP32 devkit (esp32dev, D0WD-V3) ----

// I2C -> PCA9685 + OLED (standard devkit I2C header)
constexpr int I2C_SDA = 21;
constexpr int I2C_SCL = 22;

// I2S -> MAX98357A
constexpr int I2S_BCLK = 4;
constexpr int I2S_LRC  = 5;
constexpr int I2S_DIN  = 16;

// No onboard WS2812. Most devkits carry an active-LOW single-color
// LED on GPIO2, but GPIO2 is a strapping pin (LOW at reset forces
// download mode), so it is not driven by default. To use a wired
// active-HIGH LED: set RGB_LED_PRESENT = true and RGB_LED_PIN to
// the LED pad.
constexpr bool RGB_LED_PRESENT = false;
constexpr int RGB_LED_PIN = 2;

// PCA9685 output enable (active LOW). Optional: wire OE -> GP17.
constexpr bool PCA9685_OE_WIRED = false;
constexpr int PCA9685_OE_PIN = 17;

#endif

// =====================================================
// AUDIO
// =====================================================

constexpr int SAMPLE_RATE = 44100;

// =====================================================
// PCA9685 / SERVOS
// =====================================================

constexpr uint8_t PCA9685_ADDRESS = 0x40;

constexpr int SERVO_STEP_MS = 10;

// Boot centering / sleep-pose rate (deg/s)
constexpr float SERVO_BOOT_SPEED_DEG_S = 35.0f;

// Servo pulse range
constexpr int SERVO_MIN_US = 800;
constexpr int SERVO_MAX_US = 2200;

// =====================================================
// OLED
// =====================================================

constexpr int OLED_WIDTH  = 128;
constexpr int OLED_HEIGHT = 32;
constexpr uint8_t OLED_ADDRESS = 0x3C;
