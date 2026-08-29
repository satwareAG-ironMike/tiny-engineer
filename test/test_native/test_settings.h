#pragma once

// Test function declarations for the settings validator suite
// (definitions: test_settings.cpp, run by test_main.cpp).

void testSleepTimeoutRange();
void testContinuousTimeoutRange();
void testHostnameRejectsNullEmptyAndOverlong();
void testHostnameAcceptsValidCharsAtMaxLen();
void testHostnameRejectsEdgeHyphens();
void testHostnameRejectsNonHostChars();
void testVolumeRange();
void testLoadingWhitelist();
void testAccessTokenRangeAndPrintable();
void testWifiSsidAndPasswordRange();
void testSleepTimeoutMsDerivation();
void testAccessTokenSetFlag();
