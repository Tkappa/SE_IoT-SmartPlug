#include "DeviceRoutineTask.h"

DeviceRout::DeviceRout(WattageReader * inCurrDevice){
  currDevice = inCurrDevice;
}

void DeviceRout::init(int basePeriod){
  Task::init(basePeriod);
  state=INT_getSettings;
  doublecheck=false;
  Flags::getInstance()->setDevLedCommand(flashing);
}


//enum InitStatus{INT_getSettings,INT_adjustReadingss};
void DeviceRout::tick(){
  switch (state) {
    case INT_getSettings:
      //Gets the settings from the EEPROM
      Settings::getInstance()->getEEPROMSettings();
      if(strcmp(Settings::getInstance()->getWifiSSID(),"\0")!=0){
        Flags::getInstance()->setWFhasSettings(true);
      }
      if(strcmp(Settings::getInstance()->getServerIp(),"\0")!=0){
        Flags::getInstance()->setWFhasServerSettings(true);
      }
      state=INT_adjustReadings;
    break;
    case INT_adjustReadings:
      if(currDevice->weight()){
        state=INT_setupDone;
        Flags::getInstance()->setDeviceReady(true);
        Flags::getInstance()->setDevLedCommand(on);
      }
    break;
    case INT_setupDone:
      if(!Flags::getInstance()->getIsPosting()){
        currDevice->read();
      }

      //Exceeded wattage we need to turn on
      if(Flags::getInstance()->getValueRead()>Settings::getInstance()->getMaxWattage()){
        //Used to avoid false positives
        if(doublecheck==true){
          Flags::getInstance()->setMasterOnOff(false);
        }
        doublecheck=true;
      }
      else{
        doublecheck=false;
      }
    break;
  }
}
