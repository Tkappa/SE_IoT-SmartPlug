
#include "WifiESP8266.h"

WifiESP8266::WifiESP8266(int tx,int rx){
  rxPin=rx;
  txPin=tx;
  wifiChannel= new SoftwareSerial(rxPin,txPin);

  setupStatus=ESP_getSettings;
  postDataStatus=ESPP_openSCK;
  getDataStatus=ESPG_openSCK;
  pingBackStatus=ESPR_openSCK;

  strcpy(lastCMDID,'\0');

  parser= new CommandParser();
  parser->addCommand("DSW=");//<wattage>;- Used to set the wattage
  parser->addCommand("DSR=");//<routine>;- Used to set the routine
  parser->addCommand("MM=");//<onoff>;- Used to tell the device to turn on/pff
  parser->addCommand("TM=");//<time>;- Used to teel the time to the device

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
      //Starts the wifi channel
      wifiChannel->listen();
      setupStatus=ESP_checkconn;
      triesBeforeTimeout=0;
      #ifdef ESPDEBUGVERBOSE
        Serial.println(F("Listening on wifi"));
      #endif
      return 0;
    break;
    case ESP_checkconn:
      setupBusyWaiting=0;

      completeSerialFlush();

      //Requests the current AP
      wifiChannel->println("AT+CWJAP?");

      //Will try to find the AP we want to connect to
      strcpy_P(connString,PSTR("+CWJAP:\""));
      strcat(connString,Settings::getInstance()->getWifiSSID());
      currToFind->setWord(connString);

      setupStatus=ESP_readResponse;
      #ifdef ESPDEBUGVERBOSE
        Serial.println(F("Requesting current AP"));
      #endif
      return 0;
    break;
    case ESP_readResponse:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        if(currToFind->search(c)){
          check=true;
        }
      }
      if(check){
        #ifdef ESPDEBUGVERBOSE
          Serial.println(F("Found desired AP"));
        #endif
        return 1;
      }
      else{
        //We arent connected to the desired AP
        setupStatus=ESP_disconnect;
        #ifdef ESPDEBUGVERBOSE
          Serial.println(F("DesiredAP not found"));
        #endif
        return -1;
      }
    break;
    case ESP_disconnect:
      #ifdef ESPDEBUGVERBOSE
        Serial.println(F("Sending disconnect message"));
      #endif
      wifiChannel->println("AT+CWQAP");
      setupStatus=ESP_connect;
      return 0;
    break;
    case ESP_connect:
      completeSerialFlush();

      strcpy(connString,"AT+CWJAP=\"");
      strcat(connString,Settings::getInstance()->getWifiSSID());
      strcat(connString,"\",\"");
      strcat(connString,Settings::getInstance()->getWifiPassword());
      strcat(connString,"\"");
      wifiChannel->println(connString);
      setupStatus=ESP_skipTick;
      triesBeforeTimeout=0;
      #ifdef ESPDEBUGVERBOSE
        Serial.print(F("Connecting to : "));
        Serial.print(Settings::getInstance()->getWifiSSID());
        Serial.print(F(","));
        Serial.println(Settings::getInstance()->getWifiPassword());
        Serial.println(connString);
      #endif
      return 0;
    break;
    case ESP_skipTick:
      //Need to do some busy waiting while the device connects to the AP
      if(triesBeforeTimeout++>=CONNECTTRIES){
        setupStatus=ESP_checkconn;
      }
      return 0;
    break;
  }
}


bool WifiESP8266::postData(){
  bool check= false;
  char bigBuffer[200]; //used for post request
  char midBuffer[100]; //used for small AT Commands
  char smallBuffer[10]; //used to parse int and float
  int tmpbuffersize=0;
  int tmplastsent=0;
  switch(postDataStatus){
    case ESPP_openSCK:
      //Empty the buffer
      completeSerialFlush();

      //Get the new value
      bufferValuesArray[lastEntered]=Flags::getInstance()->getValueRead();
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

      #ifdef ESPDEBUGVERBOSE
        itoa(bufferCurrentSize,smallBuffer,10);
        Serial.print(F("Buffer has size "));
        Serial.println(smallBuffer);
      #endif

      strcpy(bigBuffer,"AT+CIPSTART=\"TCP\",");
      strcat(bigBuffer,"\"");
      strcat(bigBuffer,Settings::getInstance()->getServerIp());
      strcat(bigBuffer,"\",");
      strcat(bigBuffer,Settings::getInstance()->getServerPort());

      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(bigBuffer);
      #endif
      wifiChannel->println(bigBuffer);

      //We try to find connect to handle both the cases CONNECT OK and ALREADY CONNECTED
      currToFind->setWord("CONNECT");
      postDataStatus=ESPP_checkConn;
      return false;
    break;
    case ESPP_checkConn:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        if(currToFind->search(c)){
          postDataStatus=ESPP_cipsend;
          return false;
        }
      }
      //Connect was not found , it means an error happed, we add the value to the buffer and skip this cycle
      postDataStatus=ESPP_addBuffer;
      return 0;
    break;
    case ESPP_cipsend:
      //Creates the DATA string in the midBuffer
      tmpbuffersize=bufferCurrentSize;
      tmplastsent=lastSent;
      strcpy_P(midBuffer,PSTR("id="));
      strcat(midBuffer,Settings::getInstance()->getID());
      strcat_P(midBuffer,PSTR("&key="));
      strcat(midBuffer,Settings::getInstance()->getSecretKey());
      strcat_P(midBuffer,PSTR("&values="));
      while(tmpbuffersize>=1){
        dtostrf(bufferValuesArray[tmplastsent],6,2,smallBuffer);
        strcat(midBuffer,smallBuffer);
        if(tmpbuffersize!=1){
          strcat(midBuffer,",");
        }
        tmpbuffersize--;
        tmplastsent=(tmplastsent+1)%BUFFERSIZE;
      }

      //Creates the real post string
      strcpy_P(bigBuffer,PSTR("POST "));
      strcat_P(bigBuffer,PSTR("/recv"));
      strcat_P(bigBuffer,PSTR(" HTTP/1.1\r\nHost: "));
      strcat(bigBuffer,Settings::getInstance()->getServerIp());
      strcat_P(bigBuffer,PSTR(":"));
      strcat(bigBuffer,Settings::getInstance()->getServerPort());
      strcat_P(bigBuffer,PSTR("\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length:"));
      itoa(strlen(midBuffer),smallBuffer,10);
      strcat(bigBuffer,smallBuffer);
      strcat_P(bigBuffer,PSTR("\r\n\r\n"));
      strcat(bigBuffer,midBuffer);
      strcat_P(bigBuffer,PSTR("\r\n"));

      itoa(strlen(bigBuffer)+2,smallBuffer,10);

      strcpy(midBuffer,"AT+CIPSEND=");
      strcat(midBuffer,smallBuffer);

      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(midBuffer);
      #endif

      wifiChannel->println(midBuffer);
      postDataStatus=ESPP_senddata;
      return false;
    break;
    case ESPP_senddata:

      completeSerialFlush();

      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(bigBuffer);
      #endif
      wifiChannel->println(bigBuffer);
      currToFind->setWord("200 OK");
      postDataStatus=ESPP_checkSent;
      triesBeforeTimeout=0;
      return false;
    break;
    case ESPP_checkSent:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        #ifdef ESPDEBUGPRINTRESPONSES
          Serial.print(c);
        #endif
        if(currToFind->search(c)){
          #ifdef ESPDEBUGVERBOSE
            Serial.println("Value sent succesfully");
          #endif

          //Update the buffer status values
          lastSent=lastEntered;
          postDataStatus=ESPP_openSCK;

          //Closes the connection
          wifiChannel->println("AT+CIPCLOSE");
          return true;
        }
        //Serial.print((char)wifiChannel->read());
      }
      if(triesBeforeTimeout++>=WIFITIMEOUT){
        //if ater WIFITIMEOUT Ticks we are still waiting for the message we consider it lost or error;
        postDataStatus=ESPP_addBuffer;
      }
      return false;
    break;
    case ESPP_addBuffer:
      //Error handling step, in this device we just ignore it as the buffer has already grown automatically
      postDataStatus=ESPP_openSCK;
      wifiChannel->println("AT+CIPCLOSE");
      return true;
    break;
  }
  return false;
}


bool WifiESP8266::getCommands(){
  bool check= false;
  char bigBuffer[200]; //used for post request
  char midBuffer[100]; //used for small AT Commands
  char smallBuffer[10]; //used to parse int and float
  int parsedcommand=-1;
  switch(getDataStatus){
    case ESPG_openSCK:
      completeSerialFlush();

      strcpy(bigBuffer,"AT+CIPSTART=\"TCP\",");
      strcat(bigBuffer,"\"");
      strcat(bigBuffer,Settings::getInstance()->getServerIp());
      strcat(bigBuffer,"\",");
      strcat(bigBuffer,Settings::getInstance()->getServerPort());
      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(bigBuffer);
      #endif
      wifiChannel->println(bigBuffer);
      //We try to find connect to handle both the cases CONNECT OK and ALREADY CONNECTED
      currToFind->setWord("CONNECT");
      getDataStatus=ESPG_checkConn;
      return false;
    break;
    case ESPG_checkConn:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        if(currToFind->search(c)){
          getDataStatus=ESPG_cipsend;
          return false;
        }
      }
      //CONNECT was not found , it means an error happened, we skip the operation
      getDataStatus=ESPG_addBuffer;
      return false;
    break;
    case ESPG_cipsend:
      //Creates the get string
      strcpy_P(bigBuffer,PSTR("GET "));
      strcat_P(bigBuffer,PSTR("/cmd?id="));
      strcat(bigBuffer,Settings::getInstance()->getID());
      strcat_P(bigBuffer,PSTR("&key="));
      strcat(bigBuffer,Settings::getInstance()->getSecretKey());
      strcat_P(bigBuffer,PSTR(" HTTP/1.1"));
      strcat_P(bigBuffer,PSTR("\r\n"));

      itoa(strlen(bigBuffer)+2,smallBuffer,10);
      strcpy(midBuffer,"AT+CIPSEND=");
      strcat(midBuffer,smallBuffer);

      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(midBuffer);
      #endif
      wifiChannel->println(midBuffer);
      getDataStatus=ESPG_senddata;
      return false;
    break;
    case ESPG_senddata:
      completeSerialFlush();

      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(bigBuffer);
      #endif
      wifiChannel->println(bigBuffer);

      parser->reset();
      getDataStatus=ESPG_checkSent;
      triesBeforeTimeout=0;
      return false;
    break;
    case ESPG_checkSent:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        #ifdef ESPDEBUGPRINTRESPONSES
          Serial.print(c);
        #endif
        parsedcommand=parser->parse(c);
        if(parsedcommand!=-1){
          #ifdef ESPDEBUGVERBOSE
            Serial.println(F("Revieced a command!"));
          #endif
          //The first parameter is the command ID
          /*
          parser->addCommand("DSW=");//<wattage>;- Used to set the wattage0
          parser->addCommand("DSR=");//<routine>;- Used to set the routine1
          parser->addCommand("MM=");//<onoff>;- Used to tell the device to turn on/pff2
          parser->addCommand("TM=")//<time>;- Used to teel the time to the device3
          */
          strcpy(lastCMDID,parser->getParam(0));
          switch (parsedcommand) {
            case 0: //DSW="); //"<wattage>;-"0
              Settings::getInstance()->setMaxWattage(atoi(parser->getParam(1)));
            break;
            case 1://DSR=");//<routine>;-1
              Settings::getInstance()->setRoutine(parser->getParam(1));
            break;
            case 2://MM=");//=<onoff>;-3
              if(parser->getParam(1)[0]=="T"){
                Flags::getInstance()->setMasterOnOff(true);
              }
              else{
                Flags::getInstance()->setMasterOnOff(false);
              }
            break;
            case 3://TM=");//<time>;- 6
              Flags::getInstance()->setSetupTime(atoi(parser->getParam(1)));
              Flags::getInstance()->setHasTime(true);
            break;

          }
          getDataStatus=ESPG_openSCK;
          wifiChannel->println("AT+CIPCLOSE");
          return true;
        }
      }
      if(triesBeforeTimeout++>=WIFITIMEOUT){
        //if ater WIFITIMEOUT Ticks we are still waiting for the message we consider it lost or error;
        getDataStatus=ESPG_addBuffer;
      }
      return false;
    break;
    case ESPG_addBuffer:
      //Error state, we close the connection if its open or not
      getDataStatus=ESPG_openSCK;
      wifiChannel->println("AT+CIPCLOSE");
      return true;
    break;
  }
  return false;
}

bool WifiESP8266::pingBack(){
  bool check= false;
  char bigBuffer[200]; //used for post request
  char midBuffer[100]; //used for small AT Commands
  char smallBuffer[10]; //used to parse int and float

  switch(pingBackStatus){
    case ESPR_openSCK:
      completeSerialFlush();

      strcpy(bigBuffer,"AT+CIPSTART=\"TCP\",");
      strcat(bigBuffer,"\"");
      strcat(bigBuffer,Settings::getInstance()->getServerIp());
      strcat(bigBuffer,"\",");
      strcat(bigBuffer,Settings::getInstance()->getServerPort());
      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(bigBuffer);
      #endif

      wifiChannel->println(bigBuffer);
      //We try to find connect to handle both the cases CONNECT OK and ALREADY CONNECTED
      currToFind->setWord("CONNECT");
      pingBackStatus=ESPR_checkConn;
      return false;
    break;
    case ESPR_checkConn:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        if(currToFind->search(c)){
          pingBackStatus=ESPR_cipsend;
          return false;
        }
      }
      //CONNECT was not found , it means an error happed
      pingBackStatus=ESPR_addBuffer;
      return false;
    break;
    case ESPR_cipsend:

      //Builds the DATA string
      strcpy_P(midBuffer,PSTR("id="));
      strcat(midBuffer,Settings::getInstance()->getID());
      strcat_P(midBuffer,PSTR("&key="));
      strcat(midBuffer,Settings::getInstance()->getSecretKey());
      strcat_P(midBuffer,PSTR("&cmd="));
      strcat(midBuffer,lastCMDID);

      //Creates the POST string
      strcpy_P(bigBuffer,PSTR("POST "));
      strcat_P(bigBuffer,PSTR("/confirmcmd"));
      strcat_P(bigBuffer,PSTR(" HTTP/1.1\r\nHost: "));
      strcat(bigBuffer,Settings::getInstance()->getServerIp());
      strcat_P(bigBuffer,PSTR(":"));
      strcat(bigBuffer,Settings::getInstance()->getServerPort());
      strcat_P(bigBuffer,PSTR("\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length:"));
      itoa(strlen(midBuffer),smallBuffer,10);
      strcat(bigBuffer,smallBuffer);
      strcat_P(bigBuffer,PSTR("\r\n\r\n"));
      strcat(bigBuffer,midBuffer);
      strcat_P(bigBuffer,PSTR("\r\n"));

      itoa(strlen(bigBuffer)+2,smallBuffer,10);

      strcpy(midBuffer,"AT+CIPSEND=");
      strcat(midBuffer,smallBuffer);

      #ifdef ESPDEBUGPRINTCOMMANDS
        Serial.println(midBuffer);
      #endif
      wifiChannel->println(midBuffer);
      pingBackStatus=ESPR_senddata;
      return false;
    break;
    case ESPR_senddata:

      completeSerialFlush();

      #ifdef ESPDEBUGPRINTCOMMANDS
      Serial.println(bigBuffer);
      #endif

      wifiChannel->println(bigBuffer);
      currToFind->setWord("200 OK");
      pingBackStatus=ESPR_checkSent;
      triesBeforeTimeout=0;
      return false;
    break;
    case ESPR_checkSent:
      while(wifiChannel->available()){
        char c = wifiChannel->read();
        #ifdef ESPDEBUGPRINTRESPONSES
          Serial.print(c);
        #endif
        if(currToFind->search(c)){
          #ifdef ESPDEBUGVERBOSE
            Serial.println(F("Sent confirmation with success"));
          #endif
          pingBackStatus=ESPR_openSCK;
          wifiChannel->println("AT+CIPCLOSE");
          return true;
        }
      }
      if(triesBeforeTimeout++>=WIFITIMEOUT){
        //if ater WIFITIMEOUT Ticks we are still waiting for the message we consider it lost or error;
        pingBackStatus=ESPR_addBuffer;
      }
      return false;
    break;
    case ESPR_addBuffer:
      pingBackStatus=ESPR_openSCK;
      wifiChannel->println("AT+CIPCLOSE");
      return true;
    break;
  }
  return false;
}


void WifiESP8266::completeSerialFlush(){
  //Flushes the outgoing buffer
  wifiChannel->flush();
  //Flushed the incoming buffer
  while(wifiChannel->available()){
    wifiChannel->read();
  }
}

void WifiESP8266::resetFSM(){
  setupStatus=ESP_getSettings;
  postDataStatus=ESPP_openSCK;
  getDataStatus=ESPG_openSCK;
  pingBackStatus=ESPR_openSCK;
  parser->reset();
}
