#include "BluetoothRoutineTask.h"


BluetoothRoutine::BluetoothRoutine(int tx,int rx,int state,long baud): BluetoothTask(tx,rx){
  baudRate=baud;
  statePin=state;
  ticksAlone=0;
  parser= new CommandParser();
  parser->addCommand("WS=<ssid>;<pswd>;-");
  parser->addCommand("DSW=<wattage>;-");
  parser->addCommand("DSR=<routine>;-");
  parser->addCommand("MM=<onoff>;-");
  parser->addCommand("MF=<fastmode>;-");
  parser->addCommand("SS=<serverip>;<serverport>;<DevID>;<DevKey>;-");
}


void BluetoothRoutine::init(int basePeriod){
  Task::init(basePeriod);
  begin(baudRate);
  state=BTR_waitReady;
  pinMode(statePin, INPUT);
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
      readPairState= digitalRead(statePin);
      if(readPairState==HIGH){
        btChannel->listen();
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
      readPairState= digitalRead(statePin);
      if(readPairState==LOW){
        state=BTR_turnOff;
      }
      else{
        while(btChannel->available()){
          char c= btChannel->read();
          if(parser->parse(c)!=-1){
            parser->reset();
          }
          //Serial.print(c);
          //Serial.print("-");
        }
      }
    break;
    case BTR_turnOff:
      Flags::getInstance()->setBTLedCommand(off);
      Flags::getInstance()->setBTOn(false);
      state=BTR_waitReady;
    break;
  }
}
