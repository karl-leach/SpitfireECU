// IStorage.h
#ifndef ISTORAGE_H
#define ISTORAGE_H
#include <WString.h>

class IStorage {
public:
    virtual ~IStorage() = default;
    virtual bool begin() = 0;
    virtual bool write(String key, String value) = 0;
    virtual String read(String key) = 0;

};

#endif