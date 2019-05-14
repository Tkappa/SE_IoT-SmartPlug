#include "WifiRoutineTask.h"


WifiRoutine::WifiRoutine(int tx,int rx,WifiDevice * inputDevice,long baud): WifiTask(tx,rx,inputDevice){
  baudRate=baud;
}


void WifiRoutine::init(int basePeriod){
  Task::init(basePeriod);
  begin(baudRate);
  state=WFR_hasSettings;
  currentDevice->setSerial(wifiChannel);
}

void WifiRoutine::tick(){
  Serial.print(state);
  switch (state) {
    case WFR_hasSettings:
      if(Flags::getInstance()->getWFhasSettings()){
        state=WFR_setup;
      }
    break;
    case WFR_setup:
      if(currentDevice->setup()){
        state=WFR_postData;
      }
    break;
    case WFR_postData:
      Serial.println("Qui si sta bene");
      if(currentDevice->postData()){
        state=WFR_getCommand;
      }
    break;
    case WFR_getCommand:
      if(currentDevice->getCommands()){
        state=WFR_pingServer;
      }
      break;
    /*case WFR_handleCommand:
      if(currentDevice->setup()){
        state=WFR_postData;
      }
    break;
    */
    case WFR_pingServer:
      if(currentDevice->pingBack()){
        state=WFR_postData;
      }
    break;
  }
}
