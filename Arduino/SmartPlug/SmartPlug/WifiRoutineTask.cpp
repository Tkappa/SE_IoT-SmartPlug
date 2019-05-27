#include "WifiRoutineTask.h"


WifiRoutine::WifiRoutine(WifiDevice * inputDevice){
  currentDevice=inputDevice;
  errorTollerance=0;
}


void WifiRoutine::init(int basePeriod){
  Task::init(basePeriod);
  state=WFR_hasSettings;
}

void WifiRoutine::tick(){
  int statusMsg;
  switch (state) {
    case WFR_hasSettings:
      if(Flags::getInstance()->getWFhasSettings()&&Flags::getInstance()->getWFhasServerSettings()){
        errorTollerance=0;
        state=WFR_setup;
        Flags::getInstance()->setWifiLedCommand(flashing);
      }
    break;
    case WFR_setup:
      statusMsg=currentDevice->setup();
      if(statusMsg==1){
        state=WFR_postData;
      }
      else if(statusMsg==-1){
        #ifdef WFROUTDEBUGVERBOSE
          Serial.println(F("There was an error in the connection"));
        #endif
        if(errorTollerance++>=5){
          Flags::getInstance()->setWFhasSettings(false);
          #ifdef WFROUTDEBUGVERBOSE
            Serial.println(F("Stopping trying to connect, to many errors"));
          #endif
          Flags::getInstance()->setWifiLedCommand(off);
          state=WFR_hasSettings;
        }
      }
    break;
    case WFR_postData:
      statusMsg=currentDevice->postData();
      if(statusMsg==1){
        state=WFR_getCommand;
        Flags::getInstance()->setWifiLedCommand(on);
      }
      else if(statusMsg==-2){
        //Buffer is full, the server is unreachable
      }
    break;
    case WFR_getCommand:
      //As the operation is time sensitive (The buffer fills fast) we let the device know so it can skip time consuming tasks
      Flags::getInstance()->setIsPosting(true);
      if(currentDevice->getCommands()){
        Flags::getInstance()->setIsPosting(false);
        state=WFR_pingServer;
      }
      break;
    case WFR_pingServer:

      if(currentDevice->pingBack()){
        state=WFR_postData;
      }
    break;
  }
}

void WifiRoutine::reset(){
  currentDevice->resetFSM();
  state=WFR_hasSettings;

  Flags::getInstance()->setWifiLedCommand(off);
}
