#ifndef __WATTREADER__
#define __WATTREADER__

class WattageReader{
public:
  virtual void begin();
  virtual void read();
  virtual bool weigth();
};

#endif
