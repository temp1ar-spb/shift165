#include <shift165.h>

inline void writePin(uint8_t pin, bool x) {  //fast pin write
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
inline bool readPin(uint8_t pin){ //fast pin read
    bool x;
    if (pin < 8) (x = (PIND & (1ul << pin)));
    else if (pin < 14) (x = (PINB & (1ul << (pin-8))));
    else if (pin < 20) (x = (PINC & (1ul << (pin-14))));
    return x;
}



shift165::shift165(const uint8_t dataPin, const uint8_t clockPin, const uint8_t latchPin, const uint8_t chipCount) {  // (data, clock, latch, chip amount)
    _clockPin =  clockPin;
    _dataPin  = dataPin;
    _latchPin =  latchPin;
    _chipCount =  chipCount;

    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, INPUT);
    pinMode(_latchPin, OUTPUT);

    // set pins
    writePin(_clockPin, 0);
    writePin(_dataPin, 0);
    writePin(_latchPin, 1);
}
    
uint64_t shift165::getAll() {
    _oldValues = _values;
    _values = 0;

    writePin(_latchPin, 0);
    writePin(_latchPin, 1);
   
    for (uint8_t j = _chipCount; j >0 ; j--) {
      for(uint8_t i = 0; i < 8; i++) {
        bool value = readPin(_dataPin);
        _values |= (value << (((j-1)*8) + i)); 
        writePin(_clockPin, 1);
        writePin(_clockPin, 0);
      }
    }
    return _values;
}
uint64_t shift165::getAllPrevious() {return _oldValues;}
bool shift165::getPin(uint8_t pin) {return (_values & (1ul << (_chipCount * 8 - 1 - pin)));}
bool shift165::getPinPrevious(uint8_t pin) {return (_oldValues & (1ul << (_chipCount * 8 - 1 - pin)));}
bool shift165::updated() {return getAll() != _oldValues;}
bool shift165::updatedPin(uint8_t pin) {return getPin(pin) != getPinPrevious(pin);}
// bool shift165::pressedPin(uint8_t pin) {return getPin(pin) && ~getPinPrevious(pin);}

