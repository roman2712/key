#ifndef KEY_H
#define KEY_H

#define POSITIVE 1
#define NEGATIVE 0

class key_analog
{
  private:
    byte k;
    int value_min;
	int value_max;
	
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
    key_analog(byte _k, int _value, byte _range = 50, byte _debounceDelay = 50)
    {
      k = _k;
	  value_min = _value-_range;
	  value_max = _value+_range;
      debounceDelay = _debounceDelay;
      pinMode(k, INPUT);
	  reset();
    }
    byte read(unsigned long = 0);
    byte readPop();
	void reset();
};

void key_analog::reset()
{
	buttonStatePush = lastButtonStatePush = buttonStatePop = lastButtonStatePop = readPin();
}

byte key_analog::readPin()
{
	int temp = analogRead(k);
	if (value_min < temp && temp < value_max) return true;
	else return false;
}

byte key_analog::read(unsigned long _delay)
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

byte key_analog::readPop()
{
  lastButtonStatePop = reading;
  reading = readPin();

  if (reading != lastButtonStatePop) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) >  delay) {
    if (reading != buttonStatePop) {
      buttonStatePop= reading;
      if (buttonStatePop == LOW) return 1;
    }
  }
  return 0;
}

#endif