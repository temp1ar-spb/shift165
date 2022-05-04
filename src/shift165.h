#pragma once
#include <Arduino.h>

class shift165
{
public:
    shift165(const uint8_t dataPin, const uint8_t clockPin, const uint8_t latchPin, const uint8_t chipCount);
    uint64_t getAll();
    uint64_t getAllPrevious();
    bool getPin(uint8_t pin);
    bool getPinPrevious(uint8_t pin);
    bool updated();
    bool updatedPin(uint8_t pin);
    
private:
    uint8_t _clockPin;
    uint8_t _dataPin;
    uint8_t _latchPin;
    uint8_t _chipCount;
    uint64_t _values;
    uint64_t _oldValues;
};


