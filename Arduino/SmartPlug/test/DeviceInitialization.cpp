/*#include "Deviceinitialization.h"

DeviceInit::DeviceInit(WattageReader * inCurrDevice){
  currDevice = inCurrDevice;
}

void DeviceInit::init(int basePeriod){
  Task::init(basePeriod);

  state=BTI_checkBtn;
}


//enum InitStatus{INT_getSettings,INT_adjustReadingss};
void DeviceInit::tick(){
  switch (state) {
    case INT_getSettings:
      //If the button was pressed and the BT device isn't ready yet it means that it needs to initialize
      if(!Flags::getInstance()->getBTReady()&&Flags::getInstance()->getBTBtnRequest()){
        state=INT_adjustReadings;
        currDevice->resetFSM();
      }
    break;
    case INT_adjustReadings:
      //Turns on the device
      if(currDevice->powerOn()){
        state=INT_setupDone;
      };
    break;
    case INT_setupDone:
    break;
  }
}
*/
