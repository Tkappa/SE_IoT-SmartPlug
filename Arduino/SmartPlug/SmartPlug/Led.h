#ifndef __LED__
#define __LED__


class Led{
public:
  virtual void begin();
  virtual void turnOn();
  virtual void turnOff();
  virtual void toggle();
  virtual bool getStatus();

protected:
  bool status;
};

#endif
