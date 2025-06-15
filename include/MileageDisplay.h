#ifndef MILEAGE_DISPLAY_H
#define MILEAGE_DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Images.h>

#define IMAGE_WIDTH 24  // Adjust based on your byte array size
#define IMAGE_HEIGHT 32
#define IMAGE_GAP 1     // 1-pixel gap between images

class MileageDisplay {
private:
    Adafruit_SSD1306 display;
    int sdaPin, sclPin;
    unsigned long lastUpdateTime;
    int lastCount;
    int mileage;
    


    int xPos[5]= {1,26,51,76,101};

    const unsigned char* NUMBERS[10] = {
	N0,
	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,
	N9
    };

    void getDigits(int num, int* digits);

public:
    MileageDisplay(int width = 128, int height = 32);

    bool initialize();
    void setMileage(int initialMileage = 0);
    void calculateMileage(int count);
    void updateDisplay();


    
};

#endif
