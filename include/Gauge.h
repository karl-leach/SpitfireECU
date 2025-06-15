#ifndef GAUGE_H
#define GAUGE_H

#include <Servo.h>

class Gauge {
private:
    Servo servo;
    int pin;
    int minDegrees;
    int maxDegrees;
    int minValue;
    int maxValue;
    bool isDigital;
    bool Clockwise;

    int currentValue;
    int targetValue;

public:
    // Constructor
    Gauge(int _pin, int _minDegrees, int _maxDegrees, int _minValue, int _maxValue, bool _isDigital, bool _Clockwise);

    // Function to set the gauge value
    void setValue(float value);

    void initialize();

};

#endif
