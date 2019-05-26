#include "DeviceRoutineTask.h"

DeviceRout::DeviceRout(WattageReader * inCurrDevice){
  currDevice = inCurrDevice;
}

void DeviceRout::init(int basePeriod){
  Task::init(basePeriod);
  state=INT_getSettings;

  Flags::getInstance()->setDevLedCommand(flashing);
}


//enum InitStatus{INT_getSettings,INT_adjustReadingss};
void DeviceRout::tick(){
  switch (state) {
    case INT_getSettings:
      Settings::getInstance()->getEEPROMSettings();
      if(strcmp(Settings::getInstance()->getWifiSSID(),"\0")!=0){
        //Has wifi settings
        Flags::getInstance()->setWFhasSettings(true);
      }
      if(strcmp(Settings::getInstance()->getServerIp(),"\0")!=0){
        Flags::getInstance()->setWFhasServerSettings(true);
      }
      state=INT_adjustReadings;
    break;
    case INT_adjustReadings:
      if(currDevice->weigth()){
        state=INT_setupDone;
        Flags::getInstance()->setDeviceReady(true);
        Flags::getInstance()->setDevLedCommand(on);
      }
    break;
    case INT_setupDone:

      currDevice->read();

      //Exceeded wattage!!
      if(Flags::getInstance()->getValueRead()>Settings::getInstance()->getMaxWattage()){
        Flags::getInstance()->setMasterOnOff(false);
      }
    break;
  }
}
