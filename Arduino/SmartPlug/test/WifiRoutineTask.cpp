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
        Flags::getInstance()->setWifiLedCommand(on);
      }
      else if(statusMsg==-1){
        Serial.println(F("Error!:("));
        if(errorTollerance++>=3){
          Flags::getInstance()->setWFhasSettings(false);
          Serial.println(F("Basta ora mi sono annoiato"));
          Flags::getInstance()->setWifiLedCommand(off);
          state=WFR_hasSettings;
        }
      }
    break;
    case WFR_postData:
      Serial.println(F("Qui si sta bene"));
      if(currentDevice->postData()==1){
        state=WFR_getCommand;
      }
    break;
    case WFR_getCommand:
      if(currentDevice->getCommands()){
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
