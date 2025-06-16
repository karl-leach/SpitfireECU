


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <MileageDisplay.h>
#include "Gauge.h"
#include <IStorage.h>
#include <i2cEEPROMStorage.h>
#include <SpeedSensor.h>
#include <Config.h>
#include<TachoSensor.h>




int Miles = 12345;
hw_timer_t *timer = NULL;
hw_timer_t *timer2 = NULL;
static unsigned long lastTimerUpdate = 0;


float currentValue =0;

#ifdef ODOMETER
    MileageDisplay display;
#endif

#ifdef STORAGE
    IStorage* storage;
#endif

#ifdef EEPROMSTORAGE
    i2cEEPROMStorage eepromStorage(EEPROM_ADDRESS,EEPROM_SIZE);
#endif

#ifdef SPEEDO
Gauge speedo(SPEED_OUTPUT_PIN,SPEED_OUTPUT_MIN_ANGLE,SPEED_OUTPUT_MAX_ANGLE,SPEED_OUTPUT_MIN_VALUE,SPEED_OUTPUT_MAX_VALUE,SPEED_OUTPUT_ISDIGITAL,SPEED_OUTPUT_CLOCKWISE);
SpeedSensor speedinput;
#endif

#ifdef TACHO
Gauge tacho(TACHO_OUTPUT_PIN,TACHO_OUTPUT_MIN_ANGLE,TACHO_OUTPUT_MAX_ANGLE,TACHO_OUTPUT_MIN_VALUE,TACHO_OUTPUT_MAX_VALUE,TACHO_OUTPUT_ISDIGITAL,TACHO_OUTPUT_CLOCKWISE);
TachoSensor tachoinput(PPR);
#endif


void testinterrupt()
{
    speedinput.handleInterrupt();
}

// Placeholder byte arrays (replace with your actual images)
void setup() {
    Serial.begin(115200);

    //#define TIMERTEST
    #ifdef TIMERTEST
    timer = timerBegin(0, 80, true); // Use Timer 0, prescaler 80 (1µs resolution)
    timerAttachInterrupt(timer, &speedinput.handleInterrupt, true);
    timerAlarmWrite(timer, 3000, true); // 2500µs = 2.5ms (400 Hz)
    timerAlarmEnable(timer);

    timer2 = timerBegin(0, 80, true);
    timerAttachInterrupt(timer2, &tachoinput.handleInterrupt, true);
    timerAlarmWrite(timer2, 5000, true); // Start with 5ms interval
    timerAlarmEnable(timer2);

    #endif
    
    
    #ifdef EEPROMSTORAGE
        eepromStorage.SetRomSize(EEPROM_SIZE);
        storage = &eepromStorage;
        storage->begin();
    #endif

    #ifdef SDSTORAGE
        // for future SD card version initialise here.
    #endif

   
    #ifdef STORAGE
        if(storage->read("Mileage").length() > 0)
        {
            Miles = storage->read("Mileage").toInt();
        }
        else
        {
            Miles = 100000;
            storage->write("Mileage", String(Miles));
        }
    #endif

    #ifdef ODOMETER
    display.initialize();
    display.setMileage(Miles);
    #endif

    #ifdef SPEEDO
        speedo.initialize();
        speedo.setValue(SPEED_OUTPUT_MIN_VALUE);
        pinMode(SPEED_SENSOR_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), speedinput.handleInterrupt, FALLING);

    #endif


    #ifdef TACHO
        tacho.initialize();
        pinMode(TACHO_SENSOR_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(TACHO_SENSOR_PIN), speedinput.handleInterrupt, FALLING);

    #endif

    delay(1500);



}



void loop() 
{
    delay(250);
    

    Serial.print("Miles:");
    Serial.println(Miles);

    #ifdef SPEEDO
        int result = speedinput.getCurrentSpeed();
        speedo.setValue(result);
        Serial.println(result);
    #endif

    #ifdef TACHO
        int tacho_result = tachoinput.getRPM();
        tacho.setValue(tacho_result);
        Serial.println(tacho_result);
    #endif

    #ifdef ODOMETER
     int increase = speedinput.getDistanceTravelled();
     if(increase > 0)
     {
        Miles += increase;
        #ifdef STORAGE
             storage->write("Mileage", String(Miles));
        #endif
        
        #ifdef ODOMETER
            display.setMileage(Miles);
        #endif
     }
    #endif


}

