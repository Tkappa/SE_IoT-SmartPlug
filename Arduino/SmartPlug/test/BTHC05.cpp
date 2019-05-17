
#include "BTHC05.h";


BTHC05::BTHC05(int tx,int rx,int invccPin,int inatPin,int inStatePin)/*:BluetoothDevice(tx,rx)*/{

  txPin=tx;
  rxPin=rx;
  btChannel= new SoftwareSerial(txPin, rxPin);
  vccPin=invccPin;
  atPin=inatPin;
  statePin=inStatePin;

  pinMode(vccPin, OUTPUT);
  pinMode(atPin, OUTPUT);
  pinMode(statePin, INPUT);


  setupStatus= BTHCSS_enterAtMode;

  parser= new CommandParser();
  parser->addCommand((char*)F("WS=<ssid>;<pswd>;-"));
  parser->addCommand((char*)F("DSW=<wattage>;-"));
  parser->addCommand((char*)F("DSR=<routine>;-"));
  parser->addCommand((char*)F("MM=<onoff>;-"));
  parser->addCommand((char*)F("MF=<fastmode>;-"));
  parser->addCommand((char*)F("SS=<serverip>;<serverport>;<DevID>;<DevKey>;-"));
  currToFind = new WordFinder();

}

bool BTHC05::powerOn(){
  digitalWrite(vccPin,HIGH);
  //digitalWrite(atPin,HIGH);
  return true;
}
bool BTHC05::setup(){
  switch (setupStatus) {
    case BTHCSS_enterAtMode:
      //Serial.println("BTHCSS_enterAtMode");
      //Sets it to AT MODE so it can recieve status messagess
      digitalWrite(atPin,HIGH);
      btChannel->listen();
      //btChannel->println((const char*)F("AT+STATE?"));
      //Serial.println("Printing");
      btChannel->println("AT+STATE?");//(const char*)F("AT+STATE?"));
      currToFind->setWord("PAIRABLE");
      setupStatus=BTHCSS_waitForPairable;
      //while(btChannel->available()){
      //  char c = btChannel->read();
      //  Serial.print(c);
      //}
      Serial.println("Printed");
      return false;
    break;
    case BTHCSS_waitForPairable:
    Serial.println("Jackpot");
      //Will wait until the device is in the PAIRABLE status
      //TODO: limited amout of tries before turning the device off?
      //btChannel->println("AT");
      //Serial.println("BTHCSS_waitForPairable");
      btChannel->println("AT+STATE");
      while(btChannel->available()){
        char c = btChannel->read();
        Serial.print(c);
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
  Serial.print(statusStatePin);
  if(statusStatePin==HIGH){
    return true;
  }
  return false;
}

bool BTHC05::handleMessages(){
  while(btChannel->available()){
    char c= btChannel->read();
    Serial.print(c);
    if(parser->parse(c)!=-1){
      Serial.print(F("Found!"));
      parser->reset();
    }
  }
}


void BTHC05::begin(long baud){
  //BluetoothDevice::begin(baud);

  btChannel->begin(baud);
  baudRate=baud;
  btChannel->listen();
  digitalWrite(vccPin,LOW);
  digitalWrite(atPin,LOW);
}
