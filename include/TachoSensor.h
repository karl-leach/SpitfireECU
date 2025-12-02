#ifndef TACHOSENSOR_H
#define TACHOSENSOR_H

#include <Arduino.h>

class TachoSensor
{
private:
    const float pulsesPerRevolution; // Adjust based on your engine setup

public:

    volatile static unsigned long pulseCount;
    volatile static unsigned long lastPulseTime;
    volatile static unsigned long lastPulseDuration;

    static void handleInterrupt();
    TachoSensor(float ppr); // Constructor with pulses per revolution
    float getRPM();
};

#endif