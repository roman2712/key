#ifndef KEY_H
#define KEY_H

#define POSITIVE 1
#define NEGATIVE 0

struct flags_struct{
	unsigned reading : 1;
    unsigned buttonStatePush : 1;
    unsigned lastButtonStatePush : 1;
	unsigned buttonStatePop : 1;
	unsigned lastButtonStatePop : 1;
};

class key
{
  private:
	flags_struct flags;
	
    byte debounceDelay;
    unsigned long lastDebounceTime;
	
	virtual byte readPin();

  public:
    key(byte _debounceDelay = 50)
    {
      debounceDelay = _debounceDelay;
    }
	
	byte readPush(unsigned long = 0);
    byte readPop(unsigned long = 0);
	void reset();
};

void key::reset()
{
	flags.buttonStatePush = flags.lastButtonStatePush = flags.buttonStatePop = flags.lastButtonStatePop = readPin();
}

byte key::readPush(unsigned long _delay)
{
  flags.lastButtonStatePush = flags.reading;
  flags.reading = readPin();

  if (flags.reading != flags.lastButtonStatePush) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) >  debounceDelay + (flags.reading ^ HIGH ? 0 : _delay)) {
    if (flags.reading != flags.buttonStatePush) {
      flags.buttonStatePush = flags.reading;
      if (flags.buttonStatePush == HIGH) return 1;
    }
  }
  return 0;
}

byte key::readPop(unsigned long _delay)
{
  flags.lastButtonStatePop = flags.reading;
  flags.reading = readPin();

  if (flags.reading != flags.lastButtonStatePop) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) >  debounceDelay + (flags.reading ^ LOW ? 0 : _delay)) {
    if (flags.reading != flags.buttonStatePop) {
      flags.buttonStatePop = flags.reading;
      if (flags.buttonStatePop == LOW) return 1;
    }
  }
  return 0;
}

class digitalKey : public key
{
	private:
	byte k;
    byte p;
	
	byte readPin();
	
	public:
	digitalKey(byte _k, byte _p = NEGATIVE, byte mode = INPUT_PULLUP, byte _debounceDelay = 50) : key(_debounceDelay)
	{
	  k = _k;
      p = _p;
	  pinMode(k, mode);
	  reset();
	}
};

byte digitalKey::readPin()
{
  if (p) return digitalRead(k);
  else return !digitalRead(k);
}

class analogKey : public key
{
	private:
	byte k;
    int value_min;
	int value_max;
	
	byte readPin();
	
	public:
	analogKey(byte _k, int _value, byte _range = 50, byte _debounceDelay = 50): key(_debounceDelay)
	{
	  k = _k;
	  value_min = _value-_range;
	  value_max = _value+_range;
	  pinMode(k, INPUT);
	  reset();
	}
};

byte analogKey::readPin()
{
	int temp = analogRead(k);
	if (value_min < temp && temp < value_max) return true;
	else return false;
}

#endif