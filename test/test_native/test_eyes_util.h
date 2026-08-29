#pragma once

// Test function declarations for the eyes geometry suite
// (definitions: test_eyes_util.cpp, run by test_main.cpp).

void testEyeWithHeightVerticalCentering();
void testRenderEyeFullOpenIsIdentity();
void testRenderEyeHalfOpen();
void testRenderEyeClosedIsFlatLineAtTop();
void testRenderEyeBlinkClosedAmount();
void testPhaseProgress();
void testMoveProgress();
void testLerpInt();
