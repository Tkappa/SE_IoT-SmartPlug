#ifndef __WIFIESP8266__
#define __WIFIESP8266__

#include "WifiDevice.h"
#include "DeviceSettings.h"
#include "DeviceFlags.h"
#include "SoftwareSerial.h"
#include "WordFinder.h"
#include "CommandParser.h"
#include <avr/pgmspace.h>

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
  bool checkConnection();
  bool disconnect();
  bool connect();

  int postData();
  bool getCommands();
  bool pingBack();
  int freeRum () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
};

private:
  ESPsetup setupStatus;
  ESPpostData postDataStatus;
  ESPgetData getDataStatus;
  ESPpingRecv pingBackStatus;

  char serverip[30];
  char serverport[5];
  char secretKey[5];
  char ID[3];
  char wifiSSID[20];
  char wifiPassword[20];
  char lastCMDID[5];

  float bufferValuesArray[BUFFERSIZE];
  uint8_t lastSent;
  uint8_t lastEntered;
  float currentReading;
  int bufferCurrentSize;

    //  String dd = "Content-Length:" + String(data.length());"


  WordFinder * currToFind;
  CommandParser * parser;
  int setupBusyWaiting;
  int triesBeforeTimeout;
};

#endif
