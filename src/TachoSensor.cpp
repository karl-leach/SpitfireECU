#include "TachoSensor.h"

volatile unsigned long TachoSensor::pulseCount = 0;
volatile unsigned long TachoSensor::lastPulseTime = 0;
volatile unsigned long TachoSensor::lastPulseDuration = 0;

void IRAM_ATTR TachoSensor::handleInterrupt()
{
    unsigned long currentTime = micros();
    lastPulseDuration = currentTime - lastPulseTime;
    lastPulseTime = currentTime;
    pulseCount++;
}

TachoSensor::TachoSensor(float ppr) : pulsesPerRevolution(ppr) {}

float TachoSensor::getRPM()
{
    if (lastPulseDuration == 0)
        return 0.0;

    float timePerRevolution = (lastPulseDuration * pulsesPerRevolution) / pulseCount;
    return (60.0 * 1000000.0) / timePerRevolution;
}