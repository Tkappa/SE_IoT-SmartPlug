#ifndef __WIFIDEVICE__
#define __WIFIDEVICE__

#include "Arduino.h"
#include "SoftwareSerial.h"

class WifiDevice{
public:
virtual bool setup();
virtual void setSerial(SoftwareSerial * inwifiChannel);
virtual bool checkConnection();
virtual bool disconnect();
virtual bool connect();

virtual bool postData();
virtual bool getCommands();
virtual bool pingBack();

protected:
  SoftwareSerial * wifiChannel;

};

#endif
