#include <shift165.h>

shift165::shift165(const uint8_t dataPin, const uint8_t clockPin, const uint8_t latchPin, const uint8_t chipCount); {  // (data, clock, latch, chip amount)
    _clockPin =  clockPin;
    _dataPin  = dataPin;
    _latchPin =  latchPin;
    _chipCount =  chipCount;

    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);

    // set pins low
    WriteP(clockPin, 0);
    WriteP(dataPin, 0);
    WriteP(latchPin, 0);
}
    
uint64_t shift165::getAll() {
    _oldValues = _values;
    _values = 0;

    writeP(latchPin, 0);
    writeP(latchPin, 1);

    for(uint8_t i = 1; i <= (chipCount * 8); i++) {
        bool value = readP(dataPin);
        _values |= (value << (chipCount * 8 - i));
        writeP(clockPin, 1);
        writeP(clockPin, 0);
    }
    return _values;
}
uint64_t shift165::getAllPrevious() {return _oldValues}
bool shift165::getPin(uint8_t pin) {return (_values & (1ul << (_chipCount * 8 - pin)))}
bool shift165::getPinPrevious(uint8_t pin) {return (_oldValues & (1ul << (_chipCount * 8 - pin)))}
bool shift165::updated() {return getAll() != _oldValues}
bool shift165::updatedPin(uint8_t pin) {return getPin(pin) != getPinPrevious(pin)}

void writeP(uint8_t pin, bool x) {  //fast pin write
  if (x == 1) {
    if (pin < 8) PORTD |= 1 << pin;
    else if (pin < 14) PORTB |= 1 << (pin - 8);
    else if (pin < 20) PORTC |= 1 << (pin - 14);
    else return;
  }
  else {
     if (pin < 8) PORTD &= ~(1 << pin);
    else if (pin < 14) PORTB &= ~(1 << (pin - 8));
    else if (pin < 20) PORTC &= ~(1 << (pin - 14));
    else return;
  }
  
}
bool readP(uint8_t pin){ //fast pin read
    bool x;
    if (pin < 8) (x = (PIND & (1ul << pin)));
    else if (pin < 14) (x = (PINB & (1ul << (pin-8))));
    else if (pin < 20) (x = (PINC & (1ul << (pin-14))));
    return x;
}