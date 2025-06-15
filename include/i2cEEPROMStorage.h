#ifndef I2CEEPROMSTORAGE_H
#define I2CEEPROMSTORAGE_H

#include <IStorage.h>
#include <Wire.h>
#include <AT24Cxx.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>



class i2cEEPROMStorage : public IStorage
{
private:
    AT24Cxx eeprom;
    int _ROMSIZE;
    TaskHandle_t writeTaskHandle;
    QueueHandle_t writeQueue;
    
    bool exists(String key);
    int findKey(String key);
    int findNextAvailableAddress();
    void writeToAddress(int addr, String data);
    String readFromAddress(int addr);

public:
    i2cEEPROMStorage(uint8_t address = 0x50, uint8_t size = (uint8_t)256);
    bool begin() override;
    void SetRomSize(int size);
    bool write(String key, String value) override;
    String read(String key) override;
    void flash();
};

#endif