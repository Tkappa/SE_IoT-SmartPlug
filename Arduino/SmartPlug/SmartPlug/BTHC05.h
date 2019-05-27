#ifndef __BTHC05__
#define __BTHC05__

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "WordFinder.h"
#include "CommandParser.h"
#include "BluetoothDevice.h"
#include "DeviceSettings.h"
#include "DeviceFlags.h"

//debug Readings
//#define BTVERBOSE


enum HCSetup {BTHCSS_enterAtMode,BTHCSS_waitForPairable};

class BTHC05 : public BluetoothDevice{
public:

BTHC05(int tx,int rx,int invccPin,int inatPin,int inStatePin);

bool setup();
bool powerOn();
bool isReady();
bool powerOff();


void resetFSM();
bool isPaired();

bool handleMessages();

void begin(long baud);

private:
  byte vccPin;
  byte atPin;
  byte statePin;
  HCSetup setupStatus;
  WordFinder * currToFind;
  CommandParser * parser;

};

#endif
