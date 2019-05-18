
#include "DeviceFlags.h"

Flags* Flags::instance;

Flags * Flags::getInstance(){
  if(instance==0){
    instance= new Flags();
  }
  return instance;
}

Flags::Flags(){
  BTBtnRequest=false;
  BTReady=false;
  BTLedCommand=off;
  valueRead=0.0;

}

bool Flags::getBTBtnRequest(){
  return BTBtnRequest;
}
bool Flags::getBTReady(){
  return BTReady;
}
bool Flags::getBTOn(){
  return BTOn;
}
LEDMODE Flags::getBTLedCommand(){
  return BTLedCommand;
}

void Flags::setBTBtnRequest(bool in){
  BTBtnRequest=in;
}
void Flags::setBTReady(bool in){
  BTReady=in;
}void Flags::setBTOn(bool in){
  BTOn=in;
}
void Flags::setBTLedCommand(LEDMODE in){
  BTLedCommand=in;
}


void Flags::setWFhasSettings(bool in){
  WFhasSettings=in;
}
bool Flags::getWFhasSettings(){
  return WFhasSettings;
}

void Flags::setValueRead(float in){
  valueRead=in;
}
float Flags::getValueRead(){
  return valueRead;
}
