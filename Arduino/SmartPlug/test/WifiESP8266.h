#ifndef __WIFIESP8266__
#define __WIFIESP8266__

#include "WifiDevice.h"
#include "DeviceSettings.h"
#include "DeviceFlags.h"
#include "SoftwareSerial.h"
#include "WordFinder.h"
#include "CommandParser.h"

enum ESPsetup {ESP_getSettings,ESP_checkconn,ESP_disconnect,ESP_connect,ESP_skipTick};

class WifiESP8266 : public WifiDevice{

public:
  WifiESP8266();

  void setSerial(SoftwareSerial * inwifiChannel);
  bool setup();
  bool checkConnection();
  bool disconnect();
  bool connect();

  bool postData();
  bool getCommands();
  bool pingBack();

private:
  ESPsetup setupStatus;

  char serverip[30];
  char serverport[10];
  char secretKey[10];
  char ID[5];
  char wifiSSID[100];
  char wifiPassword[20];

//  SoftwareSerial * wifiChannel;

    WordFinder * currToFind;
    CommandParser * parser;

};

#endif
