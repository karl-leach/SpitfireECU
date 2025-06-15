#include "Gauge.h"

// Constructor Implementation
Gauge::Gauge(int _pin, int _minDegrees, int _maxDegrees, int _minValue, int _maxValue, bool _isDigital, bool _Clockwise)
    : pin(_pin), minDegrees(_minDegrees), maxDegrees(_maxDegrees),
      minValue(_minValue), maxValue(_maxValue), isDigital(_isDigital), Clockwise(_Clockwise) {
    
    if (_isDigital) {
        servo.attach(pin, servo.CHANNEL_NOT_ATTACHED, _minDegrees, _maxDegrees, 
                     500, 2500, 200);
    } else {
        servo.attach(pin);
    }

   
}

// Function to set the gauge value
void Gauge::setValue(float value) {
    // Clamp value within display range
    if (value < minValue) value = minValue;
    if (value > maxValue) value = maxValue;

    int angle = 0;

    if(!Clockwise)
        targetValue = map(value, maxValue, minValue, minDegrees, maxDegrees);
    else
        targetValue = map(value, minValue, maxValue, minDegrees, maxDegrees);

    
    servo.write(targetValue); // Analog control with PWM
}

void Gauge::initialize()
{
    setValue(0);
    delay(1000);
}
