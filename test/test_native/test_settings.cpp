// Host-side Unity tests for the pure settings validators and accessors.
//
// settings.cpp is host-compilable (no Arduino includes), but its accessors
// reference state owned by settings_storage.cpp, which is Arduino-bound.
// The g_* globals below are test stubs for that state only; the validators
// under test are pure functions and use none of it.
//
// Unity 2.x API: TEST_ASSERT_* (no __LINE__ argument).

#include <cstring>

#include "settings.h"
#include "settings_internal.h"

#include <unity.h>

// Stubs: state owned by settings_storage.cpp (not host-buildable).
uint32_t g_sleepTimeoutMin = SETTINGS_DEFAULT_SLEEP_TIMEOUT_MIN;
char g_hostname[SETTINGS_HOSTNAME_MAX_LEN + 1] = {};
char g_bootHostname[SETTINGS_HOSTNAME_MAX_LEN + 1] = {};
uint8_t g_volume = SETTINGS_DEFAULT_VOLUME;
bool g_welcome = SETTINGS_DEFAULT_WELCOME;
bool g_serialLog = SETTINGS_DEFAULT_SERIAL_LOG;
uint32_t g_continuousTimeoutMin =
  SETTINGS_DEFAULT_CONTINUOUS_TIMEOUT_MIN;
char g_loading[SETTINGS_LOADING_MAX_LEN + 1] = {};
char g_accessToken[SETTINGS_ACCESS_TOKEN_MAX_LEN + 1] = {};
char g_wifiSsid[SETTINGS_WIFI_SSID_MAX_LEN + 1] = {};
char g_wifiPassword[SETTINGS_WIFI_PASSWORD_MAX_LEN + 1] = {};

#include "settings.cpp"

void testSleepTimeoutRange() {
  TEST_ASSERT_FALSE(settingsValidateSleepTimeout(0));
  TEST_ASSERT_TRUE(settingsValidateSleepTimeout(1));
  TEST_ASSERT_TRUE(settingsValidateSleepTimeout(1440));
  TEST_ASSERT_FALSE(settingsValidateSleepTimeout(1441));
}

void testContinuousTimeoutRange() {
  TEST_ASSERT_FALSE(settingsValidateContinuousTimeout(0));
  TEST_ASSERT_TRUE(settingsValidateContinuousTimeout(1));
  TEST_ASSERT_TRUE(settingsValidateContinuousTimeout(1440));
  TEST_ASSERT_FALSE(settingsValidateContinuousTimeout(1441));
}

void testHostnameRejectsNullEmptyAndOverlong() {
  TEST_ASSERT_FALSE(settingsValidateHostname(nullptr));
  TEST_ASSERT_FALSE(settingsValidateHostname(""));

  char overlong[33];
  memset(overlong, 'a', 32);
  overlong[32] = '\0';
  TEST_ASSERT_FALSE(settingsValidateHostname(overlong));
}

void testHostnameAcceptsValidCharsAtMaxLen() {
  char atMax[32];
  memset(atMax, 'a', 31);
  atMax[31] = '\0';
  TEST_ASSERT_TRUE(settingsValidateHostname(atMax));

  TEST_ASSERT_TRUE(settingsValidateHostname("tiny-engineer"));
  TEST_ASSERT_TRUE(settingsValidateHostname("A-Z0"));
}

void testHostnameRejectsEdgeHyphens() {
  TEST_ASSERT_FALSE(settingsValidateHostname("-abc"));
  TEST_ASSERT_FALSE(settingsValidateHostname("abc-"));
  TEST_ASSERT_TRUE(settingsValidateHostname("a-b"));
}

void testHostnameRejectsNonHostChars() {
  TEST_ASSERT_FALSE(settingsValidateHostname("a b"));
  TEST_ASSERT_FALSE(settingsValidateHostname("a_b"));
  TEST_ASSERT_FALSE(settingsValidateHostname("ab.c"));
}

void testVolumeRange() {
  TEST_ASSERT_TRUE(settingsValidateVolume(0));
  TEST_ASSERT_TRUE(settingsValidateVolume(100));
  TEST_ASSERT_FALSE(settingsValidateVolume(101));
  TEST_ASSERT_FALSE(settingsValidateVolume(255));
}

void testLoadingWhitelist() {
  TEST_ASSERT_FALSE(settingsValidateLoading(nullptr));
  TEST_ASSERT_FALSE(settingsValidateLoading(""));
  TEST_ASSERT_TRUE(settingsValidateLoading("progress"));
  TEST_ASSERT_TRUE(settingsValidateLoading("sleep_inertia"));
  TEST_ASSERT_FALSE(settingsValidateLoading("Progress"));
}

void testAccessTokenRangeAndPrintable() {
  TEST_ASSERT_FALSE(settingsValidateAccessToken(nullptr));
  TEST_ASSERT_TRUE(settingsValidateAccessToken(""));

  char atMax[65];
  memset(atMax, 'a', 64);
  atMax[64] = '\0';
  TEST_ASSERT_TRUE(settingsValidateAccessToken(atMax));

  char overlong[66];
  memset(overlong, 'a', 65);
  overlong[65] = '\0';
  TEST_ASSERT_FALSE(settingsValidateAccessToken(overlong));

  TEST_ASSERT_TRUE(settingsValidateAccessToken("~ !"));
  TEST_ASSERT_FALSE(settingsValidateAccessToken("a\x1f" "b"));
}

void testWifiSsidAndPasswordRange() {
  TEST_ASSERT_FALSE(settingsValidateWifiSsid(""));
  TEST_ASSERT_TRUE(settingsValidateWifiSsid("a"));

  char ssidAtMax[33];
  memset(ssidAtMax, 'a', 32);
  ssidAtMax[32] = '\0';
  TEST_ASSERT_TRUE(settingsValidateWifiSsid(ssidAtMax));

  char ssidOverlong[34];
  memset(ssidOverlong, 'a', 33);
  ssidOverlong[33] = '\0';
  TEST_ASSERT_FALSE(settingsValidateWifiSsid(ssidOverlong));

  TEST_ASSERT_TRUE(settingsValidateWifiPassword(""));

  char passAtMax[64];
  memset(passAtMax, 'p', 63);
  passAtMax[63] = '\0';
  TEST_ASSERT_TRUE(settingsValidateWifiPassword(passAtMax));

  char passOverlong[65];
  memset(passOverlong, 'p', 64);
  passOverlong[64] = '\0';
  TEST_ASSERT_FALSE(settingsValidateWifiPassword(passOverlong));
}

void testSleepTimeoutMsDerivation() {
  g_sleepTimeoutMin = 2;
  TEST_ASSERT_EQUAL_UINT32(2 * 60 * 1000, settingsSleepTimeoutMs());

  g_sleepTimeoutMin = SETTINGS_DEFAULT_SLEEP_TIMEOUT_MIN;
  TEST_ASSERT_EQUAL_UINT32(
    SETTINGS_DEFAULT_SLEEP_TIMEOUT_MIN,
    settingsSleepTimeoutMin()
  );
}

void testAccessTokenSetFlag() {
  g_accessToken[0] = '\0';
  TEST_ASSERT_FALSE(settingsAccessTokenSet());

  strcpy(g_accessToken, "token");
  TEST_ASSERT_TRUE(settingsAccessTokenSet());
  g_accessToken[0] = '\0';
}
