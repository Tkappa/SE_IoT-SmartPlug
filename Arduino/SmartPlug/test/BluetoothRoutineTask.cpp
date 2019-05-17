#include "BluetoothRoutineTask.h"


BluetoothRoutine::BluetoothRoutine(BluetoothDevice * inCurrDevice){
  ticksAlone=0;
}


void BluetoothRoutine::init(int basePeriod){
  Task::init(basePeriod);
  state=BTR_waitReady;
}

void BluetoothRoutine::tick(){
  int readPairState;
  switch (state) {
    case BTR_waitReady:
      if(Flags::getInstance()->getBTReady()){
        ticksAlone=0;
        Flags::getInstance()->setBTOn(true);
        //Sets the BT led to signal that the BT device is ready for pairing
        Flags::getInstance()->setBTLedCommand(flashing);
        state=BTR_waitPair;
      }
    break;
    case BTR_waitPair:
      Serial.println("hauhau");
      if(currDevice->isPaired()){
        Flags::getInstance()->setBTLedCommand(on);
        state=BTR_handleMessages;
      }
      else{
        // if the device waits MAXTRIES times without being paired it will be turned off, as there doesen't seem any device that wants to pair
        if(++ticksAlone>=MAXTRIES){
          state=BTR_turnOff;
        }
      }
    break;
    case BTR_handleMessages:
      //handle messagess
      if(!currDevice->isPaired()){
        state=BTR_turnOff;
      }
      else{
        currDevice->handleMessages();
        }
    break;
    case BTR_turnOff:
      Flags::getInstance()->setBTLedCommand(off);
      Flags::getInstance()->setBTOn(false);
      state=BTR_waitReady;
    break;
  }
}
