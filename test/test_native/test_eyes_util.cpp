// Host-side Unity tests for the pure eyes geometry/timing math.
//
// display/eyes/core/util.cpp is self-contained (stdint + eyes.h + constants.h),
// so it is included directly and compiled on the host.
//
// Unity 2.x API: TEST_ASSERT_* (no __LINE__ argument).

#include <unity.h>

#include "display/eyes/core/util.cpp"

void testEyeWithHeightVerticalCentering() {
  // EYE_CENTER_Y = DEFAULT_LEFT.y + height/2 = 9 + 7 = 16
  const Eye e = eyes::eyeWithHeight(20, 24, 14);
  TEST_ASSERT_EQUAL_INT16(20, e.x);
  TEST_ASSERT_EQUAL_INT16(9, e.y);
  TEST_ASSERT_EQUAL_INT16(24, e.width);
  TEST_ASSERT_EQUAL_INT16(14, e.height);

  const Eye e10 = eyes::eyeWithHeight(20, 24, 10);
  TEST_ASSERT_EQUAL_INT16(11, e10.y);
  TEST_ASSERT_EQUAL_INT16(10, e10.height);

  // Odd height: integer division, y = 16 - 0 = 16
  const Eye e1 = eyes::eyeWithHeight(20, 24, 1);
  TEST_ASSERT_EQUAL_INT16(16, e1.y);
}

void testRenderEyeFullOpenIsIdentity() {
  const Eye base = {20, 9, 24, 14};
  const Eye r = eyes::renderEye(base, 1.0f);
  TEST_ASSERT_EQUAL_INT16(base.x, r.x);
  TEST_ASSERT_EQUAL_INT16(base.y, r.y);
  TEST_ASSERT_EQUAL_INT16(base.width, r.width);
  TEST_ASSERT_EQUAL_INT16(base.height, r.height);
}

void testRenderEyeHalfOpen() {
  const Eye base = {20, 9, 24, 14};
  // 14 * 0.5 = 7; y = 9 + (14 - 7) / 2 = 12
  const Eye r = eyes::renderEye(base, 0.5f);
  TEST_ASSERT_EQUAL_INT16(20, r.x);
  TEST_ASSERT_EQUAL_INT16(12, r.y);
  TEST_ASSERT_EQUAL_INT16(24, r.width);
  TEST_ASSERT_EQUAL_INT16(7, r.height);
}

void testRenderEyeClosedIsFlatLineAtTop() {
  const Eye base = {20, 9, 24, 14};
  const Eye r = eyes::renderEye(base, 0.0f);
  TEST_ASSERT_EQUAL_INT16(20, r.x);
  TEST_ASSERT_EQUAL_INT16(9, r.y);
  TEST_ASSERT_EQUAL_INT16(24, r.width);
  TEST_ASSERT_EQUAL_INT16(0, r.height);
}

void testRenderEyeBlinkClosedAmount() {
  const Eye base = {20, 9, 24, 14};
  // 14 * 0.12 = 1.68 -> 1; y = 9 + (14 - 1) / 2 = 15
  const Eye r = eyes::renderEye(base, eyes::BLINK_CLOSED_AMOUNT);
  TEST_ASSERT_EQUAL_INT16(15, r.y);
  TEST_ASSERT_EQUAL_INT16(1, r.height);
}

void testPhaseProgress() {
  TEST_ASSERT_EQUAL_FLOAT(1.0f, eyes::phaseProgress(100, 0, 0));
  TEST_ASSERT_EQUAL_FLOAT(1.0f, eyes::phaseProgress(2000, 0, 1000));
  TEST_ASSERT_EQUAL_FLOAT(0.5f, eyes::phaseProgress(1000, 0, 2000));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, eyes::phaseProgress(500, 100, 500));
}

void testMoveProgress() {
  TEST_ASSERT_EQUAL_FLOAT(1.0f, eyes::moveProgress(100, 0, 0));
  TEST_ASSERT_EQUAL_FLOAT(0.0f, eyes::moveProgress(500, 1000, 1000));
  TEST_ASSERT_EQUAL_FLOAT(0.25f, eyes::moveProgress(1250, 1000, 1000));
  TEST_ASSERT_EQUAL_FLOAT(1.0f, eyes::moveProgress(2000, 1000, 1000));
}

void testLerpInt() {
  TEST_ASSERT_EQUAL_INT16(5, eyes::lerpInt(5, 10, 0.0f));
  TEST_ASSERT_EQUAL_INT16(10, eyes::lerpInt(5, 10, 1.0f));
  TEST_ASSERT_EQUAL_INT16(5, eyes::lerpInt(0, 10, 0.5f));
  TEST_ASSERT_EQUAL_INT16(-5, eyes::lerpInt(-10, 10, 0.25f));
  TEST_ASSERT_EQUAL_INT16(5, eyes::lerpInt(10, 0, 0.5f));
}
