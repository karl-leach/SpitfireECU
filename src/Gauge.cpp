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
void Gauge::setValue(float value, bool smoothed) {
    // Clamp value within display range
    if (value < minValue) value = minValue;
    if (value > maxValue) value = maxValue;

    int newAngle = 0;

    if (!Clockwise)
        newAngle = map(value, maxValue, minValue, minDegrees, maxDegrees);
    else
        newAngle = map(value, minValue, maxValue, minDegrees, maxDegrees);

    // --- Optional smoothing ---
    if (smoothed) 
    {
        smoothedValue[indexPos] = newAngle;
        indexPos = (indexPos + 1) % 10;
        int sum = smoothedValue[0] + smoothedValue[1] + smoothedValue[2] + smoothedValue[3] + smoothedValue[4] + smoothedValue[5] + smoothedValue[6] + smoothedValue[7] + smoothedValue[8] + smoothedValue[9];
        targetValue = sum / 10;
    }

    targetValue = newAngle;
    servo.write(targetValue); // Analog control with PWM
}

void Gauge::initialize()
{
    setValue(0,false);
    delay(1000);
}
