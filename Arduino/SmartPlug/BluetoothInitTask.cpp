#include "BluetoothInitTask.h"

BluetoothInit::BluetoothInit(BluetoothDevice * inCurrDevice){
  currDevice = inCurrDevice;
}

void BluetoothInit::init(int basePeriod){
  Task::init(basePeriod);

  state=BTI_checkBtn;
}
void BluetoothInit::tick(){
  switch (state) {
    case BTI_checkBtn:
      //If the button was pressed and the BT device isn't ready yet it means that it needs to initialize
      if(!Flags::getInstance()->getBTReady()&&Flags::getInstance()->getBTBtnRequest()){
        //In case it was posting remove the flag
        Flags::getInstance()->setIsPosting(false);
        state=BTI_powerOn;
        currDevice->resetFSM();
      }
    break;
    case BTI_powerOn:
      //Turns on the device
      if(currDevice->powerOn()){
        state=BTI_waitReady;
      };
    break;
    case BTI_waitReady:
      //Waits for it to be pairable
      if(currDevice->setup()){
        state=BTI_setReady;
      };
    break;
    case BTI_setReady:
      //Tells the other parts of the device that the BT device is ready
      Flags::getInstance()->setBTReady(true);
      //Returns the task to the a position where it will be stuck until the BT device is off again
      state= BTI_waitBtFinish;
    break;
    case BTI_waitBtFinish:
      //Waits until the Device is unpaired
      if(!Flags::getInstance()->getBTOn()){
        Flags::getInstance()->setBTReady(false);
        state = BTI_turnOff;
      }
    break;
    case BTI_turnOff:
      //Tells the other parts of the code that the bt device is unpaired
      Flags::getInstance()->setBTLedCommand(off);
      if(currDevice->powerOff()){
        Flags::getInstance()->setBTBtnRequest(false);
        state= BTI_checkBtn;
      }
    break;
  }
}
