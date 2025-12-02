#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <Arduino.h>

class SpeedSensor
{
    private:

        float fractionalMiles = 0.0;

      
        

    public:
        volatile static unsigned long counter;
        volatile static unsigned long lastpulseTime;
        volatile static unsigned long lastPulseDuration;
        volatile static bool hasSensed;
        static void handleInterrupt();
        SpeedSensor();
        float getCurrentSpeed();
        float getDistanceTravelled();
};

#endif