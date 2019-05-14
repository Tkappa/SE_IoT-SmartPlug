#include "DeviceSettings.h"

Settings* Settings::instance;

char* Settings::getServerIp(){
  return serverip;
}
char* Settings::getServerPort(){
  return serverport;
}
/*bool Settings::getRoutinePos(int pos){
  return routine[pos];
}*/
float Settings::getMaxWattage(){
  return maxWattage;
}
char* Settings::getSecretKey(){
  return secretKey;
}
char* Settings::getID(){
  return ID;
}

void Settings::setServerIp(char* ip){
  strcpy(serverip,ip);
}
void Settings::setServerPort(char* port){
  strcpy(serverport,port);
}
/*void Settings::setRoutine(bool inroutine[ROUTINELEN]){
  for(int i;i<ROUTINELEN;i++){
    routine[i]=inroutine[i];
  }
}*/
void Settings::setMaxWattage(int wattage){
  maxWattage= wattage;
}
void Settings::setSecretKey(char* key){
  strcpy(secretKey,key);
}
void Settings::setID(char* inid){
  strcpy(ID,inid);
}

Settings::Settings(){
  Serial.println("Inizio costruttore");
  for(int i =0;i<ROUTINELEN;i++){
    //routine[i]=true;
    this->setRoutineAt(i,true);
  }
  maxWattage=10000;
  Serial.println("Fine costruttore");
}

Settings * Settings::getInstance(){
  if(instance==0){
    instance= new Settings();
  }
  return instance;
}


char* Settings::getWifiPassword(){
  return wifiPassword;
}
char* Settings::getWifiSSID(){
  return wifiSSID;
}

void Settings::setWifiSSID(char* ssid){
  strcpy(wifiSSID,ssid);
}
void Settings::setWifiPassword(char* psw){
  strcpy(wifiPassword,psw);
  Serial.print(wifiPassword);
}
