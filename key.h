#ifndef KEY_H
#define KEY_H

#define POSITIVE 1
#define NEGATIVE 0

class key
{
  private:
    byte k;
    byte p;
    byte PINRead = 1;
    byte keyState = 0;
    byte keyStateOld = 0;
    byte keyStatePress = 0;
    unsigned int pressTime;
    byte readPin();
  public:
    key(byte _k, byte _p)
    {
      k = _k;
      p = _p;
      PINRead = !p;
      pinMode(k, INPUT_PULLUP);
    }
    byte read(byte n = 0);
    byte readPop(byte n = 0);
};

byte key::readPin()
{
  if (digitalRead(k) != PINRead) {
    delay(50);
    PINRead = digitalRead(k);
  }
  if (p) return PINRead;
  else return !PINRead;
}

byte key::read(byte n)
{
  keyStateOld = keyState;

  keyState = readPin();
  if (keyState == HIGH && keyState  != keyStateOld)
  {
    if (n == 0) return 1;
    keyStatePress = 1;
    pressTime = millis();
  }
  if (keyState == HIGH)
  {
    if (keyStatePress && ((millis() - pressTime) > n * 1000))
    {
      keyStatePress = 0;
      return 1;
    }
  }
  return 0;
}

byte key::readPop(byte n)
{
  keyStateOld = keyState;
  
  keyState = readPin();
  if (keyState == HIGH && keyState  != keyStateOld) pressTime = millis();
  if (keyState == LOW && keyState != keyStateOld && (millis() - pressTime) > n * 1000) return 1;
  return 0;
}

#endif
