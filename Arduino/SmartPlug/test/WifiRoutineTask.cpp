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
      if(Flags::getInstance()->getWFhasSettings()){
        errorTollerance=0;
        state=WFR_setup;
      }
    break;
    case WFR_setup:
      statusMsg=currentDevice->setup();
      if(statusMsg==1){
        state=WFR_postData;
      }
      else if(statusMsg==-1){
        Serial.println(F("Error!:("));
        if(errorTollerance++>=3){
          Flags::getInstance()->setWFhasSettings(false);
          Serial.println(F("Basta ora mi sono annoiato"));
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
