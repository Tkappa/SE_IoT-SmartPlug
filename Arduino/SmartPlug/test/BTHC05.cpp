
#include "BTHC05.h"


BTHC05::BTHC05(int tx,int rx,int invccPin,int inatPin,int inStatePin)/*:BluetoothDevice(tx,rx)*/{

  txPin=tx;
  rxPin=rx;
  btChannel= new SoftwareSerial(rxPin,txPin);
  vccPin=invccPin;
  atPin=inatPin;
  statePin=inStatePin;

  pinMode(vccPin, OUTPUT);
  pinMode(atPin, OUTPUT);
  pinMode(statePin, INPUT);


  setupStatus= BTHCSS_enterAtMode;

  parser= new CommandParser();

  parser->addCommand("WS=");//<ssid>;<pswd>;
  parser->addCommand("DSW="); //"<wattage>;-"
  parser->addCommand("DSR=");//<routine>;-
  parser->addCommand("MM=");//=<onoff>;-
  parser->addCommand("MF=");//<fastmode>;-
  parser->addCommand("SS=");//<serverip>;<serverport>;<DevID>;<DevKey>
  currToFind = new WordFinder();

}


bool BTHC05::powerOn(){
  digitalWrite(vccPin,HIGH);
  return true;
}
bool BTHC05::setup(){
  switch (setupStatus) {
    case BTHCSS_enterAtMode:
      //Sets it to AT MODE so it can recieve status messagess
      digitalWrite(atPin,HIGH);
      btChannel->listen();
      btChannel->println("AT+STATE?");
      currToFind->setWord("PAIRABLE");
      setupStatus=BTHCSS_waitForPairable;
      return false;
    break;
    case BTHCSS_waitForPairable:
      //Will wait until the device is in the PAIRABLE status
      //TODO: limited amout of tries before turning the device off?
      btChannel->println("AT+STATE");
      while(btChannel->available()){
        char c = btChannel->read();
        #ifdef DEBUG
          Serial.print(c);
        #endif
        if(currToFind->search(c)){//btChannel->read())){
          btChannel->flush();
          return true;
        }
      }
      return false;
    break;
  }

}
bool BTHC05::powerOff(){
  digitalWrite(vccPin,LOW);
  digitalWrite(atPin,LOW);
  return true;
}

void BTHC05::resetFSM(){
  setupStatus=BTHCSS_enterAtMode;
}



bool BTHC05::isPaired(){
  int statusStatePin= digitalRead(statePin);
  if(statusStatePin==HIGH){
    return true;
  }
  return false;
}

bool BTHC05::handleMessages(){
  while(btChannel->available()){
    char c= btChannel->read();
    if(parser->parse(c)!=-1){
      parser->reset();
    }
  }
}


void BTHC05::begin(long baud){
  BluetoothDevice::begin(baud);
  digitalWrite(vccPin,LOW);
  digitalWrite(atPin,LOW);
}
