#include "SpeedSensor.h"
#include <Config.h>

volatile unsigned long SpeedSensor::counter = 0;
volatile unsigned long SpeedSensor::lastpulseTime = 0;
volatile unsigned long SpeedSensor::lastPulseDuration = 0;

SpeedSensor::SpeedSensor()
{
    counter = 0;
    lastpulseTime = 0;
    lastPulseDuration = 0;
}

void IRAM_ATTR SpeedSensor::handleInterrupt()
{
    static int pulsecount = 0;
    pulsecount++;

    if(pulsecount == 4)
    {
        counter++;
        pulsecount = 0;
        lastPulseDuration = millis() - lastpulseTime;
        lastpulseTime = millis();
    }
}

float SpeedSensor::getCurrentSpeed()
{
    if(lastPulseDuration > 0)
    {
    float rotationsPerSecond = 1000.0 / lastPulseDuration;
    float speed = (1.53 * rotationsPerSecond * TIME_CONVERSION_FACTOR) / FEET_PER_MILE;
    return speed;
    }
    return 0;

}

float SpeedSensor::getDistanceTravelled()
{
    float miles = ((float)counter / PULSES_PER_MILE) + fractionalMiles;
    float wholeMiles = floor(miles);
    fractionalMiles = miles - wholeMiles;
    if(wholeMiles>0)
        counter = 0;
    return wholeMiles;
}