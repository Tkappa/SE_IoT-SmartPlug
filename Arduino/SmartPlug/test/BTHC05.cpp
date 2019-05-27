
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
  parser->addCommand("TM=");//<time>;-
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
          //Turn of AT mode so we can send messages to the device once paired
          digitalWrite(atPin,LOW);
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


/*
Server
parser->addCommand("WS=");//<ssid>;<pswd>;0
parser->addCommand("DSW="); //"<wattage>;-"1
parser->addCommand("DSR=");//<routine>;-2
parser->addCommand("MM=");//=<onoff>;-3
parser->addCommand("MF=");//<fastmode>;-4
parser->addCommand("SS=");//<serverip>;<serverport>;<DevID>;<DevKey>5
parser->addCommand("TM=");//<time>;- 6
*/

bool BTHC05::handleMessages(){
  char smallBuffer[10];
  while(btChannel->available()){
    char c= btChannel->read();
    int commandparsed=parser->parse(c);
    if(commandparsed!=-1){
      switch (commandparsed) {
        case 0: //WS=");//<ssid>;<pswd>;0
          Settings::getInstance()->setWifiSSID(parser->getParam(0));
          Settings::getInstance()->setWifiPassword(parser->getParam(1));
          Flags::getInstance()->setWFhasSettings(true);
        break;
        case 1: //DSW="); //"<wattage>;-"1
          Settings::getInstance()->setMaxWattage(atoi(parser->getParam(0)));
        break;
        case 2://DSR=");//<routine>;-2
          Settings::getInstance()->setRoutine(parser->getParam(0));
        break;
        case 3://MM=");//=<onoff>;-3
          if(parser->getParam(0)[0]=='1'){
            Flags::getInstance()->setMasterOnOff(true);
          }
          else{
            Flags::getInstance()->setMasterOnOff(false);
          }
        break;
        case 4://"MF=");//<fastmode>;-4
          if(parser->getParam(0)[0]=='1'){
            Flags::getInstance()->setFastModeStatus(true);
          }
          else{
            Flags::getInstance()->setFastModeStatus(false);
          }
        break;
        case 5://SS=");//<serverip>;<serverport>;<DevID>;<DevKey>5
          Settings::getInstance()->setServerIp(parser->getParam(0));
          Settings::getInstance()->setServerPort(parser->getParam(1));
          Settings::getInstance()->setID(parser->getParam(2));
          Settings::getInstance()->setSecretKey(parser->getParam(3));
          Flags::getInstance()->setWFhasServerSettings(true);
        break;
        case 6://TM=");//<time>;- 6
          Flags::getInstance()->setSetupTime(atoi(parser->getParam(0)));
          Flags::getInstance()->setHasTime(true);
        break;
      }
      parser->reset();
    }

  }
  if(Flags::getInstance()->getFastModeStatus()){
    dtostrf(Flags::getInstance()->getValueRead(),6,2,smallBuffer);
    btChannel->write(smallBuffer);
    strcat(smallBuffer,";");
    Serial.print("sent : ");
    Serial.println(smallBuffer);
  }
}


void BTHC05::begin(long baud){
  BluetoothDevice::begin(baud);
  digitalWrite(vccPin,LOW);
  digitalWrite(atPin,LOW);
}
