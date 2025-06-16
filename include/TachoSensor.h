#ifndef TACHOSENSOR_H
#define TACHOSENSOR_H

#include <Arduino.h>

class TachoSensor
{
private:
    volatile static unsigned long pulseCount;
    volatile static unsigned long lastPulseTime;
    volatile static unsigned long lastPulseDuration;

    const float pulsesPerRevolution; // Adjust based on your engine setup

public:
    static void handleInterrupt();
    TachoSensor(float ppr); // Constructor with pulses per revolution
    float getRPM();
};

#endif