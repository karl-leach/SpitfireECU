


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
#include <WiFi.h>




unsigned long lastTimerUpdate = 0;

bool low_oil_warning_on = false;

float currentValue =0;

int currentSpeed;
int currentRPM;
int currentMiles;
int currentOilPressure;
float currentBatteryLevel;
int currentFuelLevel;
int currentTemperature;

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

#ifdef OIL_PRESSURE_GAUGE
Gauge OilGauge(OIL_PRESSURE_GAUGE_PIN,45,135,OIL_PRESSURE_MIN_VALUE,OIL_PRESSURE_MAX_VALUE,OIL_PRESSURE_IS_DIGITAL, OIL_PRESSURE_CLOCKWISE);
#endif

#ifdef BATTERY_CHARGE_GAUGE
Gauge BattGauge(BATTERY_GAUGE_PIN,45,135,MIN_BATTERY_VALUE,MAX_BATTERY_VALUE,BATTERY_SENSOR_IS_DIGITAL, BATTERY_LEVEL_CLOCKWISE);
#endif

#ifdef FUEL_LEVEL_GAUGE
Gauge FuelGauge(FUEL_LEVEL_GAUGE_PIN,45,135,FUEL_LEVEL_MIN_VALUE,FUEL_LEVEL_MAX_VALUE,FUEL_SENSOR_IS_DIGITAL, FUEL_LEVEL_CLOCKWISE);
#endif

#ifdef TEMP_LEVEL_GAUGE
Gauge TempGauge(TEMP_LEVEL_SENSOR_PIN,45,135,TEMP_LEVEL_MIN_VALUE,TEMP_LEVEL_MAX_VALUE,TEMP_SENSOR_IS_DIGITAL, TEMP_LEVEL_CLOCKWISE);
#endif


void testinterrupt()
{
    speedinput.handleInterrupt();
}

// Placeholder byte arrays (replace with your actual images)
void setup() {

    WiFi.mode(WIFI_OFF); // Disable WiFi to save power
    btStop();

    Serial.begin(115200);

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
            currentMiles = storage->read("Mileage").toInt();
        }
        else
        {
            currentMiles = 000000;
            storage->write("Mileage", String(currentMiles));
        }
    #endif

    #ifdef ODOMETER
    display.initialize();
    display.setMileage(currentMiles);
    #endif

    #ifdef SPEEDO
        speedo.initialize();
        speedo.setValue(SPEED_OUTPUT_MIN_VALUE, false);
        pinMode(SPEED_SENSOR_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), speedinput.handleInterrupt, FALLING);

    #endif

    #ifdef OIL_PRESSURE_GAUGE
        OilGauge.initialize();
    #endif

    #ifdef BATTERY_CHARGE_GAUGE
        BattGauge.initialize();
    #endif

    #ifdef FUEL_LEVEL_GAUGE
        FuelGauge.initialize();
    #endif

    #ifdef TEMP_LEVEL_GAUGE
        TempGauge.initialize();
    #endif


    #ifdef TACHO
        tacho.initialize();
        pinMode(TACHO_SENSOR_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(TACHO_SENSOR_PIN), speedinput.handleInterrupt, FALLING);

    #endif

    delay(500);
}

void loop() 
{
    delay(10);

    #ifdef SPEEDO
        currentSpeed = speedinput.getCurrentSpeed();
        speedo.setValue(currentSpeed,false);
        Serial.println(currentSpeed);
    #endif

    #ifdef TACHO
        int currentRPM = tachoinput.getRPM();
        tacho.setValue(currentRPM,false);
        Serial.println(currentRPM);
    #endif

    #ifdef ODOMETER
     int increase = speedinput.getDistanceTravelled();
     if(increase > 0)
     {
        currentMiles += increase;
        #ifdef STORAGE
             storage->write("Mileage", String(currentMiles));
        #endif
        
        #ifdef ODOMETER
            display.setMileage(currentMiles);
        #endif
     }
    #endif

    #ifdef OIL_PRESSURE_GAUGE
    int value = analogRead(OIL_PRESSURE_SENSOR_PIN);
    currentOilPressure = map(value,OIL_PRESSURE_MIN_READING, OIL_PRESSURE_MAX_READING,OIL_PRESSURE_MIN_VALUE, OIL_PRESSURE_MAX_VALUE);
    OilGauge.setValue(currentOilPressure,true);
        #ifdef PANELLIGHTS
            #ifdef OIL_LED_PIN
                if(currentOilPressure < OIL_PRESSURE_WARNING_THRESHOLD)
                {
                    low_oil_warning_on = true;
                    digitalWrite(OIL_LED_PIN,HIGH);
                }
            #endif
        #endif
    #endif

    #ifdef BATTERY_CHARGE_GAUGE
    value = analogRead(BATTERY_SENSOR_PIN);
    Serial.print("Battery ADC:");
    Serial.println(value);  
    
    currentBatteryLevel = map(value,MIN_BATTERY_READING, MAX_BATTERY_READING, MIN_BATTERY_VALUE,MAX_BATTERY_VALUE);

    Serial.print("Battery Mapped:");
    Serial.println(currentBatteryLevel);

    BattGauge.setValue(currentBatteryLevel,true);
    #endif

    #ifdef FUEL_LEVEL_GAUGE
    value = analogRead(FUEL_LEVEL_SENSOR_PIN);
    currentFuelLevel = map(value,FUEL_LEVEL_MIN_READING, FUEL_LEVEL_MAX_READING, FUEL_LEVEL_MIN_VALUE,FUEL_LEVEL_MAX_VALUE);
    FuelGauge.setValue(currentFuelLevel,true);
    #endif

    #ifdef TEMP_LEVEL_GAUGE
    value = analogRead(FUEL_LEVEL_SENSOR_PIN);
    currentTemperature = map(value,FUEL_LEVEL_MIN_READING, FUEL_LEVEL_MAX_READING, FUEL_LEVEL_MIN_VALUE,FUEL_LEVEL_MAX_VALUE);
    TempGauge.setValue(currentTemperature,true);
    #endif

    #ifdef IGN_LED_PIN
        #ifdef TACHO
            bool isRunning = tachoinput.getRPM() > 100;  

            if(!isRunning)
            {
                digitalWrite(IGN_LED_PIN, HIGH);
            }
            else
            {
                digitalWrite(IGN_LED_PIN, LOW);
            }

        #endif
    #endif

    if(millis() - lastTimerUpdate >= 5000)
    {
        lastTimerUpdate = millis();
        //future logging here.
    }

}

