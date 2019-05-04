#include "DeviceSettings.h"

Settings* Settings::instance;

String Settings::getServerIp(){
  return serverip;
}
String Settings::getServerPort(){
  return serverport;
}
bool Settings::getRoutinePos(int pos){
  return routine[pos];
}
float Settings::getMaxWattage(){
  return maxWattage;
}
int Settings::getSecretKey(){
  return secretKey;
}
int Settings::getID(){
  return ID;
}

void Settings::setServerIp(String ip){
  serverip=ip;
}
void Settings::setServerPort(String port){
  serverport=port;
}
void Settings::setRoutine(bool inroutine[ROUTINELEN]){
  for(int i;i<ROUTINELEN;i++){
    routine[i]=inroutine[i];
  }
}
void Settings::setMaxWattage(int wattage){
  maxWattage= wattage;
}
void Settings::setSecretKey(int key){
  secretKey=key;
}
void Settings::setID(int inid){
  ID=inid;
}

Settings::Settings(){
  serverip= String();
  serverport = String();
  for(int i =0;i<ROUTINELEN;i++){
    routine[i]=true;
  }
  maxWattage=10000;
  secretKey=-1;
  ID=-1;
}

Settings * Settings::getInstance(){
  if(instance==0){
    instance= new Settings();
  }
  return instance;
}
