#include "BluetoothInitTask.h"

BluetoothInit::BluetoothInit(int tx,int rx,int vcc,int at,long baud): BluetoothTask(tx,rx){
  baudRate=baud;
  vccPin=vcc;
  atPin=at;
}

void BluetoothInit::init(int basePeriod){

  Task::init(basePeriod);
  begin(baudRate);
  state=BTI_checkBtn;
  pinMode(vccPin, OUTPUT);
  pinMode(atPin, OUTPUT);
  digitalWrite(vccPin,LOW);
  digitalWrite(atPin,LOW);
}
void BluetoothInit::tick(){
  //const char * stateString =(const char*)F("AT+STATE?");
  switch (state) {
    case BTI_checkBtn:
      //If the button was pressed and the BT device isn't ready yet it means that it needs to initialize
      if(!Flags::getInstance()->getBTReady()&&Flags::getInstance()->getBTBtnRequest()){
        state=BTI_powerOn;
      }
    break;
    case BTI_powerOn:
      //Turns on the device
      digitalWrite(vccPin,HIGH);
      state=BTI_enterAtMode;
    break;
    case BTI_enterAtMode:
      //Sets it to AT MODE so it can recieve status messagess
      digitalWrite(atPin,HIGH);
      btChannel->listen();
      btChannel->println((const char*)F("AT+STATE?"));
      currToFind = new WordFinder((char*)F("PAIRABLE"));
      state=BTI_waitForPairable;
    break;
    case BTI_waitForPairable:
      //Will wait until the device is in the PAIRABLE status
      //TODO: limited amout of tries before turning the device off?
      while(btChannel->available()){
        if(currToFind->search(btChannel->read())){
          state=BTI_setReady;
          btChannel->flush();
        }
      }
    break;
    case BTI_setReady:
      //Tells the other parts of the device that the BT device is ready
      Flags::getInstance()->setBTReady(true);
      //Returns the task to the first position where it will be stuck until the BT device is off again
      state= BTI_waitBtFinish;
    break;
    case BTI_waitBtFinish:
      if(!Flags::getInstance()->getBTOn()){
        Flags::getInstance()->setBTReady(false);
        Flags::getInstance()->setBTBtnRequest(false);
        state = BTI_turnOff;
      }
    break;
    case BTI_turnOff:

      Flags::getInstance()->setBTLedCommand(off);
      digitalWrite(vccPin,LOW);
      digitalWrite(atPin,LOW);
      state= BTI_checkBtn;
    break;
  }
}
