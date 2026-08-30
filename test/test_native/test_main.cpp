// Single Unity runner for the host-side (native) test environment.
//
// PlatformIO links every test file in this directory into one program, so
// exactly one file may define main(). The suite files (test_settings.cpp,
// test_eyes_util.cpp) define the test functions; this file runs them.

#include <unity.h>

#include "test_eyes_util.h"
#include "test_settings.h"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();

  // settings validators + accessors
  RUN_TEST(testSleepTimeoutRange);
  RUN_TEST(testContinuousTimeoutRange);
  RUN_TEST(testHostnameRejectsNullEmptyAndOverlong);
  RUN_TEST(testHostnameAcceptsValidCharsAtMaxLen);
  RUN_TEST(testHostnameRejectsEdgeHyphens);
  RUN_TEST(testHostnameRejectsNonHostChars);
  RUN_TEST(testVolumeRange);
  RUN_TEST(testLoadingWhitelist);
  RUN_TEST(testAccessTokenRangeAndPrintable);
  RUN_TEST(testWifiSsidAndPasswordRange);
  RUN_TEST(testSleepTimeoutMsDerivation);
  RUN_TEST(testAccessTokenSetFlag);

  // eyes geometry + timing math
  RUN_TEST(testEyeWithHeightVerticalCentering);
  RUN_TEST(testRenderEyeFullOpenIsIdentity);
  RUN_TEST(testRenderEyeHalfOpen);
  RUN_TEST(testRenderEyeClosedIsFlatLineAtTop);
  RUN_TEST(testRenderEyeBlinkClosedAmount);
  RUN_TEST(testPhaseProgress);
  RUN_TEST(testMoveProgress);
  RUN_TEST(testLerpInt);

  return UNITY_END();
}
