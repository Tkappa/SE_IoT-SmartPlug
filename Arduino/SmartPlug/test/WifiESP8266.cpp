
#include "WifiESP8266.h"

//enum ESPsetup {ESP_getSettings,ESP_checkconn,ESP_disconnect,ESP_connect};
  WifiESP8266::WifiESP8266(int tx,int rx){


    rxPin=rx;
    txPin=tx;
    wifiChannel= new SoftwareSerial(rxPin,txPin);
    setupStatus=ESP_getSettings;

    strcpy(serverip,'\0');
    strcpy(serverport,'\0');
    strcpy(secretKey,'\0');
    strcpy(ID,'\0');
    strcpy(wifiSSID,'\0');
    strcpy(wifiPassword,'\0');


    parser= new CommandParser();
    parser->addCommand("WS=");//<ssid>;<pswd>;-
    parser->addCommand("DSW=");//<wattage>;-
    parser->addCommand("DSR=");//<routine>;-
    parser->addCommand("MM=");//<onoff>;-
    parser->addCommand("MF=");//<fastmode>;-
    parser->addCommand("SS=");//<serverip>;<serverport>;<DevID>;<DevKey>;-

    currToFind= new WordFinder();

    setupBusyWaiting=0;
  }

  int WifiESP8266::setup(){
    bool check= false;
    char connString[140];
    Serial.println(setupStatus);

    switch (setupStatus) {
      case ESP_getSettings:

        strcpy(serverip,Settings::getInstance()->getServerIp());
        strcpy(serverport , Settings::getInstance()->getServerPort());
        strcpy(secretKey , Settings::getInstance()->getSecretKey());
        strcpy(ID , Settings::getInstance()->getID());

        strcpy(wifiSSID, Settings::getInstance()->getWifiSSID());
        strcpy(wifiPassword, Settings::getInstance()->getWifiPassword());
        Serial.print(wifiPassword);
        Serial.print(F(" vs "));
        Serial.println(Settings::getInstance()->getWifiPassword());
        setupStatus=ESP_checkconn;

        wifiChannel->listen();
        return 0;
      break;
      case ESP_checkconn:

        setupBusyWaiting=0;
        wifiChannel->flush();
        wifiChannel->println("AT+CWJAP?");
        strcpy(connString,"+CWJAP:\"");
        strcat(connString,wifiSSID);
        Serial.print("Trying to find: ");
        Serial.println(connString);
        currToFind->setWord(connString);
        setupStatus=ESP_readResponse;

      break;
      case ESP_readResponse:
        while(wifiChannel->available()){
          char c = wifiChannel->read();
          //Serial.print(c);
          if(currToFind->search(c)){
            check=true;
            Serial.println("Found");
          }
        }
        if(check){
          return 1;
        }
        else{
          Serial.println("CheckFail");
          setupStatus=ESP_disconnect;
          return -1;
        }
      break;
      case ESP_disconnect:
        Serial.println("-SendDisc");
        wifiChannel->println("AT+CWQAP");
        while(wifiChannel->available()){
          char c = wifiChannel->read();
          Serial.print(c);
        }
        setupStatus=ESP_connect;
        return 0;
      break;
      case ESP_connect:
        Serial.println(F("Connect"));
        Serial.println(wifiSSID);
        Serial.println(wifiPassword);
        strcpy(connString,"AT+CWJAP=\"");
        strcat(connString,wifiSSID);
        strcat(connString,"\",\"");
        strcat(connString,wifiPassword);
        strcat(connString,"\"");
        Serial.println(connString);
        wifiChannel->println(connString);
        setupStatus=ESP_skipTick;
        return 0;
      break;
      case ESP_skipTick:
        if(setupBusyWaiting++>=10){
          setupStatus=ESP_checkconn;
        }
        return 0;
      break;
    }
}


  bool WifiESP8266::checkConnection(){}
  bool WifiESP8266::disconnect(){}
  bool WifiESP8266::connect(){}

  bool WifiESP8266::postData(){
    return false;
  }
  bool WifiESP8266::getCommands(){}
  bool WifiESP8266::pingBack(){}
