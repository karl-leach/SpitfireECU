


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <MileageDisplay.h>
#include "Gauge.h"
#include <IStorage.h>
#include <i2cEEPROMStorage.h>

int Miles = 12345;

float currentValue =0;

MileageDisplay display;

IStorage* storage;

Gauge speedo(16,0,270,0,121,true,false);
Gauge tacho(19,0,270,0,6250,true,false);

// Placeholder byte arrays (replace with your actual images)
void setup() {
    Serial.begin(115200);
    
    i2cEEPROMStorage eepromStorage(0x50,(uint8_t)256);
    eepromStorage.flash();
    eepromStorage.SetRomSize(256);
    storage = &eepromStorage;
    storage->begin();



    if(storage->read("Mileage").length() > 0)
    {
        Miles = storage->read("Mileage").toInt();
    }
    else
    {
        Miles = 100000;
        storage->write("Mileage", String(Miles));
    }


    display.initialize();
    display.setMileage(Miles);
    speedo.initialize();
    tacho.initialize();

    speedo.setValue(0);
    tacho.setValue(0);

    delay(1500);

    speedo.setValue(100);
    tacho.setValue(6000);

    Miles++;
    storage->write("Mileage", String(Miles));

}

void loop() {
    delay(1000);
    Miles++;
    display.setMileage(Miles);
    display.updateDisplay();
    //storage->write("Mileage", String(Miles));


 


}