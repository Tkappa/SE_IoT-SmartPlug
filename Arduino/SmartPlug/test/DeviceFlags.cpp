
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
  FastModeStatus=false;
  
  BTLedCommand=off;
  WifiLedCommand=off;
  DevLedCommand=off;
  PowLedCommand=off;

  valueRead=0.0;
  hasTime=false;
  setupTime=0;
  WFhasSettings=false;
  WFhasServerSettings=false;
  masterOnOff=true;
  DeviceReady=false;

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

void Flags::setWFhasServerSettings(bool in){
  WFhasServerSettings=in;
}
bool Flags::getWFhasServerSettings(){
  return WFhasServerSettings;
}

void Flags::setValueRead(float in){
  valueRead=in;
}
float Flags::getValueRead(){
  return valueRead;
}



void Flags::setHasTime(bool in){
  hasTime=in;
}
bool Flags::getHasTime(){
  return hasTime;
}

void Flags::setSetupTime(int in){
  setupTime=in;
}
int Flags::getSetupTime(){
  return setupTime;
}

void Flags::setMasterOnOff(bool in){
  masterOnOff=in;
}
int Flags::getMasterOnOff(){
  return masterOnOff;
}


LEDMODE Flags::getWifiLedCommand(){
  return WifiLedCommand;
}

LEDMODE Flags::getDevLedCommand(){
  return DevLedCommand;
}
LEDMODE Flags::getPowLedCommand(){
  return PowLedCommand;
}
void Flags::setWifiLedCommand(LEDMODE in){
  WifiLedCommand=in;
}

void Flags::setDevLedCommand(LEDMODE in){
  DevLedCommand=in;
}
void Flags::setPowLedCommand(LEDMODE in){
  PowLedCommand=in;
}

void Flags::setDeviceReady(bool in){
  DeviceReady=in;
}
bool Flags::getDeviceReady(){
  return DeviceReady;
}

bool Flags::getFastModeStatus(){
  return FastModeStatus;
}
void Flags::setFastModeStatus(bool in){
  FastModeStatus=in;
}
