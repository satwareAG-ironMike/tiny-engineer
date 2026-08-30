#include "hardware/chip_temp.h"

#include <Arduino.h>
#include "serial_log.h"

#if defined(CONFIG_IDF_TARGET_ESP32C3)

// ESP32-C3: low-level tsens driver.
#include "driver/temperature_sensor.h"

namespace {

temperature_sensor_handle_t g_tsens = nullptr;
bool g_ready = false;

}  // namespace

void initChipTemp() {
  temperature_sensor_config_t cfg = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);

  if (temperature_sensor_install(&cfg, &g_tsens) != ESP_OK) {
    serialLogPrintln("ERROR: chip temp sensor install failed");
    g_tsens = nullptr;
    g_ready = false;
    return;
  }

  if (temperature_sensor_enable(g_tsens) != ESP_OK) {
    serialLogPrintln("ERROR: chip temp sensor enable failed");
    g_tsens = nullptr;
    g_ready = false;
    return;
  }

  g_ready = true;
  serialLogPrintln("Chip temp sensor OK");
}

bool chipTempCelsius(float* out) {
  if (!g_ready || g_tsens == nullptr || out == nullptr) {
    return false;
  }

  return temperature_sensor_get_celsius(g_tsens, out) == ESP_OK;
}

#else

// Classic ESP32: Arduino HAL sensor (tsens config API differs per chip).
// Returns NAN when the sensor is unavailable.
#include <math.h>

namespace {

bool g_ready = false;

}  // namespace

void initChipTemp() {
  const float t = temperatureRead();

  g_ready = !isnan(t);

  if (g_ready) {
    serialLogPrint("Chip temp sensor OK (");
    serialLogPrint(t, 1);
    serialLogPrintln(" C)");
  } else {
    serialLogPrintln("Chip temp sensor unavailable");
  }
}

bool chipTempCelsius(float* out) {
  if (!g_ready || out == nullptr) {
    return false;
  }

  const float t = temperatureRead();

  if (isnan(t)) {
    return false;
  }

  *out = t;
  return true;
}

#endif
