#include "BluetoothInitTask.h"

BluetoothInit::BluetoothInit(BluetoothDevice * inCurrDevice){
  currDevice = inCurrDevice;
}

void BluetoothInit::init(int basePeriod){
  Task::init(basePeriod);

  state=BTI_checkBtn;
}
void BluetoothInit::tick(){
  //const char * stateString =(const char*)F("AT+STATE?");
  switch (state) {
    case BTI_checkBtn:
      Serial.println("check");
      //If the button was pressed and the BT device isn't ready yet it means that it needs to initialize
      if(!Flags::getInstance()->getBTReady()&&Flags::getInstance()->getBTBtnRequest()){
        state=BTI_powerOn;
        currDevice->resetFSM();
      }
    break;
    case BTI_powerOn:
      //Turns on the device
        Serial.println("BTI_powerOn");
      if(currDevice->powerOn()){
        state=BTI_waitReady;
      };
    break;
    case BTI_waitReady:
      Serial.println("BTI_waitReady");
      if(currDevice->setup()){

        Serial.println("isReady");
        state=BTI_setReady;
      };
    break;
    case BTI_setReady:
      //Serial.println("BTI_setReady");
      //Tells the other parts of the device that the BT device is ready
      Flags::getInstance()->setBTReady(true);
      //Returns the task to the first position where it will be stuck until the BT device is off again
      state= BTI_waitBtFinish;
    break;
    case BTI_waitBtFinish:
      //Serial.println("BTI_waitBtFinish");
      if(!Flags::getInstance()->getBTOn()){
        Flags::getInstance()->setBTReady(false);
        Flags::getInstance()->setBTBtnRequest(false);
        state = BTI_turnOff;
      }
    break;
    case BTI_turnOff:

      Serial.println("BTI_turnOff");
      Flags::getInstance()->setBTLedCommand(off);
      if(currDevice->powerOff()){
        state= BTI_checkBtn;
      }
    break;
  }
}
