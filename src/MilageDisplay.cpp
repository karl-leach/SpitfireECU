#include <MileageDisplay.h>
#include <Images.h>

MileageDisplay::MileageDisplay(int width, int height)
    : display(width, height, &Wire), mileage(0), lastCount(0), lastUpdateTime(millis()) {}

bool MileageDisplay::initialize() {

    if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c))
    {
        Serial.println("Display Not Available");
        return false;
    }
    else
    {
        delay(1000);
        display.clearDisplay();
        display.invertDisplay(true);
        display.dim(true);
        //Serial.println("Screen Initialized");
        display.display();
        return true; 

    }

}

void MileageDisplay::setMileage(int initialMileage) {
    mileage = initialMileage;
    updateDisplay();
}

void MileageDisplay::calculateMileage(int count) {
    unsigned long currentTime = millis();
    unsigned long timeDiff = currentTime - lastUpdateTime;

    if (lastCount == 0) {
        lastCount = count;
        lastUpdateTime = currentTime;
        return;
    }

    int countDiff = count - lastCount;

    // Placeholder formula: You will define the real conversion
    int milesToAdd = countDiff * 0.01;  

    mileage += milesToAdd;
    lastCount = count;
    lastUpdateTime = currentTime;

    updateDisplay();
}

void MileageDisplay::updateDisplay() 
{

    int digits[5];

    getDigits(mileage,digits);

    display.clearDisplay();

    for(int i=0; i<5; i++)
    {
        display.drawBitmap(xPos[i],0,NUMBERS[digits[i]],IMAGE_WIDTH,IMAGE_HEIGHT,WHITE);
        //display.drawPixel(10,10,WHITE);
        //Serial.println("Drawing:");
    }

    display.display();
    
}


void MileageDisplay::getDigits(int num, int* digits) {
    for (int i = 4; i >= 0; i--) {
        digits[i] = num % 10;
        //Serial.println(digits[i]);
        num /= 10;
    }

    
}




