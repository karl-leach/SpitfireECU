#ifndef SPEEDSENSOR_H
#define SPEEDSENSOR_H

#include <Arduino.h>

class SpeedSensor
{
    private:
        volatile static unsigned long counter;
        volatile static unsigned long lastpulseTime;
        volatile static unsigned long lastPulseDuration;
        float fractionalMiles = 0.0;

      
        

    public:
        static void handleInterrupt();
        SpeedSensor();
        float getCurrentSpeed();
        float getDistanceTravelled();
};

#endif