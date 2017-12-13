#ifndef KEY_H
#define KEY_H

#define POSITIVE 1
#define NEGATIVE 0

class key
{
  private:
    byte k;
    byte p;
	
	byte reading;
		
    byte buttonStatePush;
    byte lastButtonStatePush;

	
	byte buttonStatePop;
	byte lastButtonStatePop;
	
    byte debounceDelay;
    unsigned long lastDebounceTime;
    unsigned long delay;

  public:
	byte readPin();
    key(byte _k, byte _p = NEGATIVE, byte mode = INPUT_PULLUP, byte _debounceDelay = 50)
    {
      k = _k;
      p = _p;
      debounceDelay = _debounceDelay;
      pinMode(k, mode);
    }
    byte read(unsigned long = 0);
    byte readPop();
};

byte key::readPin()
{
  if (p) return digitalRead(k);
  else return !digitalRead(k);
}

byte key::read(unsigned long _delay)
{
  lastButtonStatePush = reading;
  reading = readPin();

  if (reading != lastButtonStatePush) {
    lastDebounceTime = millis();
    if (reading == HIGH) delay = debounceDelay + _delay;
    else delay = debounceDelay;
  }

  if ((millis() - lastDebounceTime) >  delay) {
    if (reading != buttonStatePush) {
      buttonStatePush = reading;
      if (buttonStatePush == HIGH) return 1;
    }
  }
  return 0;
}

byte key::readPop()
{
  lastButtonStatePop = reading;
  reading = readPin();

  if (reading != lastButtonStatePop) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) >  delay) {
    if (reading != buttonStatePop) {
      buttonStatePop= reading;;
      if (buttonStatePop == LOW) return 1;
    }
  }
  return 0;
}

#endif