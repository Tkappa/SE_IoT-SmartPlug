
#include "WifiESP8266.h"

//enum ESPsetup {ESP_getSettings,ESP_checkconn,ESP_disconnect,ESP_connect};
  WifiESP8266::WifiESP8266(){

    setupStatus=ESP_getSettings;

    strcpy(serverip,"");
    strcpy(serverport,"");
    strcpy(secretKey,"");
    strcpy(ID,"");
    strcpy(wifiSSID,"");
    strcpy(wifiPassword,"");


    parser= new CommandParser();
    parser->addCommand("WS=<ssid>;<pswd>;-");
    parser->addCommand("DSW=<wattage>;-");
    parser->addCommand("DSR=<routine>;-");
    parser->addCommand("MM=<onoff>;-");
    parser->addCommand("MF=<fastmode>;-");
    parser->addCommand("SS=<serverip>;<serverport>;<DevID>;<DevKey>;-");
  }

  bool WifiESP8266::setup(){

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
        Serial.print(" vs ");
        Serial.println(Settings::getInstance()->getWifiPassword());
        setupStatus=ESP_checkconn;

        return false;
      break;
      case ESP_checkconn:
        wifiChannel->listen();
        wifiChannel->println("AT+CWJAP?");
        currToFind = new WordFinder(wifiSSID);
        while(wifiChannel->available()){
          char c = wifiChannel->read();
          Serial.print(c);
          if(currToFind->search(c)){
            check=true;
            Serial.print("Found");
          }
        }
        if(check){
          return true;
        }
        else{
          Serial.print("CheckFail");
          setupStatus=ESP_disconnect;
          return false;
        }

      break;
      case ESP_disconnect:
        Serial.print("-SendDisc");
        wifiChannel->println("AT+CWQAP");
        while(wifiChannel->available()){
          char c = wifiChannel->read();
          Serial.print(c);
        }
        setupStatus=ESP_connect;
        return false;
      break;
      case ESP_connect:
        Serial.println("Connect");
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
        return false;
      break;
      case ESP_skipTick:
        setupStatus=ESP_checkconn;
        return false;
      break;
    }
}




void WifiESP8266::setSerial(SoftwareSerial * inwifiChannel){
  wifiChannel=inwifiChannel;
}

  bool WifiESP8266::checkConnection(){}
  bool WifiESP8266::disconnect(){}
  bool WifiESP8266::connect(){}

  bool WifiESP8266::postData(){}
  bool WifiESP8266::getCommands(){}
  bool WifiESP8266::pingBack(){}
