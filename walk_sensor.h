#pragma once

#include <Arduino.h>

// QMI8658 pedometer used only while the walk screen is active. The gyroscope
// stays disabled to reduce current draw; the accelerometer runs at 62.5 Hz.
bool walkSensorStart();
// Samples acceleration frequently enough for the orientation-independent
// software fallback detector. Call from every game loop while walking.
void walkSensorUpdate(uint32_t nowMs);
uint32_t walkSensorSteps();
void walkSensorStop();
bool walkSensorActive();
