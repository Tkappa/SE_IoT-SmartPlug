
#include "WifiESP8266.h"

//enum ESPsetup {ESP_getSettings,ESP_checkconn,ESP_disconnect,ESP_connect};
//enum ESPpostData{ESPP_openSCK,ESPP_checkConn,ESPP_cipsend,ESPP_senddata,ESPP_checkSent,ESPP_addBuffer};
  WifiESP8266::WifiESP8266(int tx,int rx){


    rxPin=rx;
    txPin=tx;
    wifiChannel= new SoftwareSerial(rxPin,txPin);
    setupStatus=ESP_getSettings;
    postDataStatus=ESPP_openSCK;

    strcpy(serverip,'\0');
    strcpy(serverport,'\0');
    strcpy(secretKey,'\0');
    strcpy(ID,'\0');
    strcpy(wifiSSID,'\0');
    strcpy(wifiPassword,'\0');

    //Accept: *//*\r\nContent-Type: application/x-www-form-urlencoded\r\n
    parser= new CommandParser();
    parser->addCommand("WS=");//<ssid>;<pswd>;-
    parser->addCommand("DSW=");//<wattage>;-
    parser->addCommand("DSR=");//<routine>;-
    parser->addCommand("MM=");//<onoff>;-
    parser->addCommand("MF=");//<fastmode>;-
    parser->addCommand("SS=");//<serverip>;<serverport>;<DevID>;<DevKey>;-

    currToFind= new WordFinder();

    setupBusyWaiting=0;
    triesBeforeTimeout=0;


    for (int i=0;i<BUFFERSIZE;i++){
      bufferValuesArray[i]=0.0;
    }
    lastSent=0;
    lastEntered=0;
    bufferCurrentSize=0;
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
        triesBeforeTimeout=0;
        return 0;
      break;
      case ESP_checkconn:

        setupBusyWaiting=0;
        while(wifiChannel->available()){
          wifiChannel->read();
        }
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
        if(triesBeforeTimeout++>=CONNECTTRIES){
          setupStatus=ESP_checkconn;
        }
        return 0;
      break;
    }
  }


  int WifiESP8266::postData(){
    bool check= false;
    char bigBuffer[200]; //used for post request
    char midBuffer[100]; //used for small AT Commands
    char smallBuffer[10]; //used to parse int and float
    int tmpbuffersize=0;
    int tmplastsent=0;

    switch(postDataStatus){
      case ESPP_openSCK:

        currentReading=Flags::getInstance()->getValueRead();
        bufferValuesArray[lastEntered]=currentReading;
        lastEntered=(lastEntered+1)%BUFFERSIZE;
        //If the values are the same it means lastEntered looped around, so we delete a value from the buffer by incrementing lastSent
        if(lastEntered==lastSent){
          lastSent=(lastSent+1)%BUFFERSIZE;
        }
        if(lastSent>lastEntered){
          bufferCurrentSize=BUFFERSIZE-(lastSent-lastEntered);
        }
        else{
          bufferCurrentSize=lastEntered-lastSent;
        }
        itoa(bufferCurrentSize,smallBuffer,10);
        Serial.print("Buffer e' di dimensione ");
        Serial.println(smallBuffer);

        strcpy(bigBuffer,"AT+CIPSTART=\"TCP\",");
        strcat(bigBuffer,"\"");
        strcat(bigBuffer,serverip);
        strcat(bigBuffer,"\",");
        strcat(bigBuffer,serverport);
        Serial.println(bigBuffer);
        wifiChannel->println(bigBuffer);
        //Troviamo connect perchè cosi ritorniamo TRUE sia a CONNECT OK quando non c'è connessione che a ALREADY CONNECTED quando c'è gia
        currToFind->setWord("CONNECT");
        postDataStatus=ESPP_checkConn;
        return 0;
      break;
      case ESPP_checkConn:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        //Serial.print(c);
        if(currToFind->search(c)){
          check=true;
          Serial.println("Found");
          postDataStatus=ESPP_cipsend;
          return 0;
        }
      }
      //OK was not found , it means an error happed
      postDataStatus=ESPP_addBuffer;
      return 0;
      break;
      case ESPP_cipsend:

      //Costruisco i dati nel midBuffer
      tmpbuffersize=bufferCurrentSize;
      tmplastsent=lastSent;
      strcpy_P(midBuffer,PSTR("id="));
      strcat(midBuffer,ID);
      strcat_P(midBuffer,PSTR("&key="));
      strcat(midBuffer,secretKey);
      strcat_P(midBuffer,PSTR("&values="));
      while(tmpbuffersize>=1){
        Serial.print("Adding: ");
        dtostrf(bufferValuesArray[tmplastsent],6,2,smallBuffer);
        Serial.println(smallBuffer);
        strcat(midBuffer,smallBuffer);
        if(tmpbuffersize!=1){
          strcat(midBuffer,",");
        }
        tmpbuffersize--;
        tmplastsent=(tmplastsent+1)%BUFFERSIZE;
      }

      //Qui costruisco la sstringa da spedire
      strcpy_P(bigBuffer,PSTR("POST "));
      strcat_P(bigBuffer,PSTR("/recv"));
      strcat_P(bigBuffer,PSTR(" HTTP/1.1\r\nHost: "));
      strcat(bigBuffer,serverip);
      strcat_P(bigBuffer,PSTR(":8000\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length:"));
      itoa(strlen(midBuffer),smallBuffer,10);
      strcat(bigBuffer,smallBuffer);
      strcat_P(bigBuffer,PSTR("\r\n\r\n"));
      strcat(bigBuffer,midBuffer);
      strcat_P(bigBuffer,PSTR("\r\n"));

      Serial.print("dovreisendareeee");
      itoa(strlen(bigBuffer)+2,smallBuffer,10);

      strcpy(midBuffer,"AT+CIPSEND=");
      strcat(midBuffer,smallBuffer);
      Serial.println(midBuffer);
      wifiChannel->println(midBuffer);
      postDataStatus=ESPP_senddata;
      return 0;
      break;
      case ESPP_senddata:

      wifiChannel->flush();
      while(wifiChannel->available()){
        wifiChannel->read();
      }

      Serial.println(bigBuffer);
      wifiChannel->println(bigBuffer);
      currToFind->setWord("200 OK");
      postDataStatus=ESPP_checkSent;
      triesBeforeTimeout=0;
      return 0;
      break;
      case ESPP_checkSent:

      while(wifiChannel->available()){
        char c = wifiChannel->read();
        //Serial.print(c);
        if(currToFind->search(c)){
          //check=true;
          Serial.println("Mandato con successo!! :D:D");
          //postDataStatus=ESPP_cipsend;
          lastSent=lastEntered;
          //(lastSent+1)%BUFFERSIZE;
          postDataStatus=ESPP_openSCK;
          return 1;
        }
        //Serial.print((char)wifiChannel->read());
      }
      if(triesBeforeTimeout++>=WIFITIMEOUT){
        //if ater WIFITIMEOUT Ticks we are still waiting for the message we consider it lost or error;
        postDataStatus=ESPP_addBuffer;
      }
      return 0;
      break;
      case ESPP_addBuffer:
        postDataStatus=ESPP_openSCK;
      return 1;
      break;
    }
    return false;
  }
  bool WifiESP8266::getCommands(){}
  bool WifiESP8266::pingBack(){}

  bool WifiESP8266::checkConnection(){}
  bool WifiESP8266::disconnect(){}
  bool WifiESP8266::connect(){}
