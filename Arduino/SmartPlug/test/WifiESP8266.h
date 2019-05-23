#ifndef __WIFIESP8266__
#define __WIFIESP8266__

#include "WifiDevice.h"
#include "DeviceSettings.h"
#include "DeviceFlags.h"
#include "SoftwareSerial.h"
#include "WordFinder.h"
#include "CommandParser.h"
#include <avr/pgmspace.h>
#include <stdlib.h>

#define ESPDEBUGPRINTRESPONSES
#define ESPDEBUGPRINTCOMMANDS
#define ESPDEBUGVERBOSE


#define WIFITIMEOUT 5
#define CONNECTTRIES 15
#define BUFFERSIZE 10

enum ESPsetup {ESP_getSettings,ESP_checkconn,ESP_disconnect,ESP_connect,ESP_skipTick,ESP_readResponse};
enum ESPpostData{ESPP_openSCK,ESPP_checkConn,ESPP_cipsend,ESPP_senddata,ESPP_checkSent,ESPP_addBuffer};
enum ESPgetData{ESPG_openSCK,ESPG_checkConn,ESPG_cipsend,ESPG_senddata,ESPG_checkSent,ESPG_handle,ESPG_addBuffer};
enum ESPpingRecv{ESPR_openSCK,ESPR_checkConn,ESPR_cipsend,ESPR_senddata,ESPR_checkSent,ESPR_addBuffer};


class WifiESP8266 : public WifiDevice{

public:
  WifiESP8266(int tx,int rx);

  int setup();

  bool postData();
  bool getCommands();
  bool pingBack();
  
  void resetFSM();

private:
  //Finite state operations
  ESPsetup setupStatus;
  ESPpostData postDataStatus;
  ESPgetData getDataStatus;
  ESPpingRecv pingBackStatus;

  //Used check the freshness of the recieved command, and to inform the server that it has been recieved
  char lastCMDID[5];

  //Used to keep a buffer for when the server is temporanely offline
  float bufferValuesArray[BUFFERSIZE];
  uint8_t lastSent;
  uint8_t lastEntered;
  int bufferCurrentSize;

  //Useful classes for parsing input
  WordFinder * currToFind;
  CommandParser * parser;

  //Timeout variables
  int setupBusyWaiting;
  int triesBeforeTimeout;

  //Used to clean the input/output buffers of the SoftwareSerial
  void completeSerialFlush();
};

#endif
