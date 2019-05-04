#include "BluetoothInitTask.h"

BluetoothInit::BluetoothInit(int tx,int rx,int vcc,int at,long baud): BluetoothTask(tx,rx){
  baudRate=baud;
  vccPin=vcc;
  atPin=at;
}

void BluetoothInit::init(int basePeriod){

  Task::init(basePeriod);
  begin(baudRate);
  state=checkBtn;
  pinMode(vccPin, OUTPUT);
  pinMode(atPin, OUTPUT);
  digitalWrite(vccPin,LOW);
  digitalWrite(atPin,LOW);
}
void BluetoothInit::tick(){
  switch (state) {
    case checkBtn:
      //If the button was pressed and the BT device isn't ready yet it means that it needs to initialize
      if(!Flags::getInstance()->getBTReady()&&Flags::getInstance()->getBTBtnRequest()){
        state=powerOn;
      }
    break;
    case powerOn:
      //Turns on the device
      digitalWrite(vccPin,HIGH);
      state=enterAtMode;
    break;
    case enterAtMode:
      //Sets it to AT MODE so it can recieve status messagess
      digitalWrite(atPin,HIGH);
      btChannel->println("AT+STATE?");
      currToFind = new WordFinder("PAIRABLE");
      state=waitForPairable;
    break;
    case waitForPairable:
      //Will wait until the device is in the PAIRABLE status
      //TODO: limited amout of tries before turning the device off? 
      while(btChannel->available()){
        if(currToFind->search(btChannel->read())){
          state=setReady;
          btChannel->flush();
        }
      }
    break;
    case setReady:
      //Tells the other parts of the device that the BT device is ready
      Flags::getInstance()->setBTReady(true);
      //Sets the BT led to signal that the BT device is ready for pairing
      Flags::getInstance()->setBTLedCommand(flashing);
    break;
  }
}

