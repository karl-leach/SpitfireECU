#include <i2cEEPROMStorage.h>

i2cEEPROMStorage::i2cEEPROMStorage(uint8_t address, uint8_t size)
    : eeprom(address, size), _ROMSIZE(size) {}

bool i2cEEPROMStorage::begin()
{
    Wire.begin();
    return true;
}

void i2cEEPROMStorage::flash()
{
    for (int i = 0; i < eeprom.length(); i++)
    {
        eeprom.write(i, 0xFF);
    }
}

bool i2cEEPROMStorage::exists(String key)
{
    return findKey(key) != -1;
}

int i2cEEPROMStorage::findKey(String key)
{
    for (int addr = 0; addr < _ROMSIZE;)
    {
        String storedKey = readFromAddress(addr);
        if (storedKey == "") break;
        if (storedKey == key) return addr;
        addr += storedKey.length() + 1 + readFromAddress(addr + storedKey.length() + 1).length() + 1;
    }
    return -1;
}

int i2cEEPROMStorage::findNextAvailableAddress()
{
    Serial.print("ROMSIZE");
    Serial.println(_ROMSIZE);
    for (int addr = 0; addr < _ROMSIZE; addr++)
    {
        uint8_t result= eeprom.read(addr);
        Serial.print(result);
        if (result == 0xFF) return addr;
    }
    return -1;
}

void i2cEEPROMStorage::SetRomSize(int size){
    _ROMSIZE = size;
}

void i2cEEPROMStorage::writeToAddress(int addr, String data)
{
    for (int i = 0; i < data.length(); i++)
    {
        eeprom.write(addr + i, data[i]);
    }
    eeprom.write(addr + data.length(), '\0');
}

String i2cEEPROMStorage::readFromAddress(int addr)
{
    String data = "";
    char c;
    while ((c = eeprom.read(addr++)) != '\0' && addr < _ROMSIZE)
    {
        data += c;
    }
    Serial.print("READ: ");
    Serial.println(data);
    return data;
}

bool i2cEEPROMStorage::write(String key, String value)
{
    Serial.println("Writing");
    int addr = findKey(key);
    Serial.print("Addr:");
    Serial.println(addr);
    if (addr == -1)
    {
        Serial.println("Finding Next Address");
        addr = findNextAvailableAddress();
        Serial.print("Addr:");
        Serial.println(addr);
    }
    if (addr == -1)
    {
        // Add fallback here for ESP32 built-in storage if needed.
        return false;
    }
    Serial.println("Writing Address");
    writeToAddress(addr, key);
    int valueAddr = addr + key.length() + 1;
    writeToAddress(valueAddr, value);
    Serial.println("Write Good!");
    return true;
}

String i2cEEPROMStorage::read(String key)
{
    int addr = findKey(key);
    if (addr == -1)
    {
        return String();
    }
    int valueAddr = addr + key.length() + 1;
    return readFromAddress(valueAddr);
}