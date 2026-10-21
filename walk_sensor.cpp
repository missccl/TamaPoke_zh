#include "walk_sensor.h"

#include <Wire.h>
#include <SensorQMI8658.hpp>
#include <math.h>

namespace {
SensorQMI8658 qmi;
bool initialized = false;
bool active = false;
uint32_t softwareSteps = 0;
uint32_t lastSampleAt = 0;
uint32_t lastStepAt = 0;
float gravityX = 0.0f;
float gravityY = 0.0f;
float gravityZ = 1.0f;
float filteredMotion = 0.0f;
float motionPeak = 0.0f;
float motionTrough = 0.0f;
uint8_t calibrationSamples = 0;
bool motionRising = false;

void resetSoftwareDetector() {
  softwareSteps = 0;
  lastSampleAt = 0;
  lastStepAt = 0;
  gravityX = 0.0f;
  gravityY = 0.0f;
  gravityZ = 1.0f;
  filteredMotion = 0.0f;
  motionPeak = 0.0f;
  motionTrough = 0.0f;
  calibrationSamples = 0;
  motionRising = false;
}

bool initializeSensor() {
  if (!initialized) {
    // Wire is already configured by the firmware for the shared I2C bus.
    initialized = qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS);
  }
  return initialized;
}
}

bool walkSensorStart() {
  if (!initializeSensor()) return false;

  qmi.disablePedometer();
  qmi.disableGyroscope();
  qmi.disableAccelerometer();
  qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_2G,
                          SensorQMI8658::ACC_ODR_62_5Hz);
  if (!qmi.enableAccelerometer()) return false;
  // A handheld toy moves less than a watch worn on a swinging wrist. Lower
  // the QMI8658 peak thresholds and accept a walking sequence after two
  // consecutive steps. The chip still enforces a quiet time between counts.
  qmi.configPedometer(40, 110, 50, 250, 12, 2, 0, 1);
  if (!qmi.clearPedometerCounter() ||
      !qmi.enablePedometer(SensorQMI8658::INTERRUPT_PIN_DISABLE)) {
    qmi.disableAccelerometer();
    return false;
  }
  resetSoftwareDetector();
  active = true;
  return true;
}

void walkSensorUpdate(uint32_t nowMs) {
  if (!active || (lastSampleAt && nowMs - lastSampleAt < 25UL)) return;
  lastSampleAt = nowMs;

  float x = 0.0f, y = 0.0f, z = 0.0f;
  if (!qmi.getAccelerometer(x, y, z)) return;
  const float magnitude = sqrtf(x * x + y * y + z * z);
  if (magnitude < 0.05f || magnitude > 4.0f) return;

  if (calibrationSamples < 12) {
    const float alpha = calibrationSamples == 0 ? 1.0f : 0.22f;
    gravityX += (x - gravityX) * alpha;
    gravityY += (y - gravityY) * alpha;
    gravityZ += (z - gravityZ) * alpha;
    calibrationSamples++;
    return;
  }

  // Track gravity as a slowly moving 3-D vector. Scalar magnitude alone barely
  // changes when a user simply carries the device while walking; the dynamic
  // vector also sees the gentle rotation and swing of a normal hand-held gait.
  constexpr float kGravityAlpha = 0.018f;
  gravityX += (x - gravityX) * kGravityAlpha;
  gravityY += (y - gravityY) * kGravityAlpha;
  gravityZ += (z - gravityZ) * kGravityAlpha;
  const float dx = x - gravityX;
  const float dy = y - gravityY;
  const float dz = z - gravityZ;
  const float motion = sqrtf(dx * dx + dy * dy + dz * dz);
  filteredMotion = filteredMotion * 0.55f + motion * 0.45f;

  // Detect a complete motion wave. A hand-held walk can keep the motion
  // envelope elevated, but each gait cycle still has a local peak and trough.
  constexpr float kPeakThreshold = 0.035f;
  constexpr float kWaveHysteresis = 0.010f;
  constexpr uint32_t kMinimumStepMs = 320UL;
  if (!motionRising) {
    if (filteredMotion < motionTrough) motionTrough = filteredMotion;
    if (filteredMotion >= motionTrough + kWaveHysteresis) {
      motionRising = true;
      motionPeak = filteredMotion;
    }
  } else {
    if (filteredMotion > motionPeak) motionPeak = filteredMotion;
    if (filteredMotion <= motionPeak - kWaveHysteresis) {
      if (motionPeak >= kPeakThreshold &&
          (!lastStepAt || nowMs - lastStepAt >= kMinimumStepMs)) {
        if (softwareSteps < 1000UL) softwareSteps++;
        lastStepAt = nowMs;
      }
      motionRising = false;
      motionTrough = filteredMotion;
    }
  }
}

uint32_t walkSensorSteps() {
  return active ? softwareSteps : 0;
}

void walkSensorStop() {
  if (!active) return;
  qmi.disablePedometer();
  qmi.disableGyroscope();
  qmi.disableAccelerometer();
  active = false;
}

bool walkSensorActive() { return active; }
