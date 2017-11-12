#ifndef KEY_H
#define KEY_H

#define POSITIVE 1
#define NEGATIVE 0

class key
{
  private:
    byte k;
    byte p;
    byte buttonStatePush;
    byte lastButtonStatePush;
	byte readingPush;
	
	byte buttonStatePop;
	byte lastButtonStatePop;
	byte readingPop;
	
    byte debounceDelay;
    unsigned long lastDebounceTime;
    unsigned long delay;

    byte readPin();
  public:
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
  lastButtonStatePush = readingPush;
  readingPush = readPin();

  if (readingPush != lastButtonStatePush) {
    lastDebounceTime = millis();
    if (readingPush == HIGH) delay = debounceDelay + _delay;
    else delay = debounceDelay;
  }

  if ((millis() - lastDebounceTime) >  delay) {
    if (readingPush != buttonStatePush) {
      buttonStatePush = readingPush;
      if (buttonStatePush == HIGH) return 1;
    }
  }
  return 0;
}

byte key::readPop()
{
  lastButtonStatePop = readingPop;;
  readingPop = readPin();

  if (readingPop != lastButtonStatePop) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) >  delay) {
    if (readingPop != buttonStatePop) {
      buttonStatePop= readingPop;;
      if (buttonStatePop == LOW) return 1;
    }
  }
  return 0;
}

#endif
