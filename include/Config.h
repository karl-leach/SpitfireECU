#ifndef CONFIG_H
#define CONFIG_H


#define SPEEDO // comment out if not using speedo.
#ifdef SPEEDO
//Speedo output
#define SPEED_OUTPUT_PIN 16
#define SPEED_OUTPUT_MIN_ANGLE 0
#define SPEED_OUTPUT_MAX_ANGLE 270
#define SPEED_OUTPUT_MIN_VALUE 0
#define SPEED_OUTPUT_MAX_VALUE 121
#define SPEED_OUTPUT_ISDIGITAL true
#define SPEED_OUTPUT_CLOCKWISE false

#define SPEED_SENSOR_PIN 27
#define PULSES_PER_MILE 13782
#define TIME_CONVERSION_FACTOR 3600.0
#define FEET_PER_MILE 5280.0

#endif


#define TACHO
#ifdef TACHO

#define TACHO_OUTPUT_PIN 19
#define TACHO_OUTPUT_MIN_ANGLE 0
#define TACHO_OUTPUT_MAX_ANGLE 270
#define TACHO_OUTPUT_MIN_VALUE 0
#define TACHO_OUTPUT_MAX_VALUE 6200
#define TACHO_OUTPUT_ISDIGITAL true
#define TACHO_OUTPUT_CLOCKWISE false

#define TACHO_SENSOR_PIN 26
#define PPR 2

#endif


#define STORAGE
#ifdef STORAGE 
    // EEPROM STORAGE
    #define EEPROMSTORAGE
    #ifdef EEPROMSTORAGE
        #define EEPROM_ADDRESS 0x50
        #define EEPROM_SIZE 256
    #endif

    //SD STORATE
    //#define SDSTORAGE
    #ifdef SDSTORAGE


    #endif


    #if defined(EEPROMSTORAGE) && defined(SDSTORAGE)
        #error "EEPROMSTORAGE and SDSTORAGE cannot coexist!"
    #endif


#endif


#define ODOMETER
#ifdef ODOMETER

#endif



#endif