#include "DeviceSettings.h"

Settings* Settings::instance;

char* Settings::getServerIp(){
  return settingsInstance.serverip;
}
char* Settings::getServerPort(){
  return settingsInstance.serverport;
}
/*bool Settings::getRoutinePos(int pos){
  return routine[pos];
}*/
float Settings::getMaxWattage(){
  return settingsInstance.maxWattage;
}
char* Settings::getSecretKey(){
  return settingsInstance.secretKey;
}
char* Settings::getID(){
  return settingsInstance.ID;
}

void Settings::setServerIp(char* ip){
  strcpy(settingsInstance.serverip,ip);
  writeEEPROMSettings();
}
void Settings::setServerPort(char* port){
  strcpy(settingsInstance.serverport,port);
  writeEEPROMSettings();
}
void Settings::setRoutine(char* inroutine){
  strcpy(settingsInstance.routine,inroutine);
  writeEEPROMSettings();
}
void Settings::setMaxWattage(int wattage){
  settingsInstance.maxWattage= wattage;
  writeEEPROMSettings();
}
void Settings::setSecretKey(char* key){
  strcpy(settingsInstance.secretKey,key);
  writeEEPROMSettings();
}
void Settings::setID(char* inid){
  strcpy(settingsInstance.ID,inid);
  writeEEPROMSettings();
}

Settings::Settings(){
  Serial.println("Inizio costruttore");
  for(int i =0;i<ROUTINELEN;i++){
    //routine[i]=true;
    this->setRoutineAt(i,true);
  }
  settingsInstance.maxWattage=10000;
  Serial.println("Fine costruttore");
}

Settings * Settings::getInstance(){
  if(instance==0){
    instance= new Settings();
  }
  return instance;
}


char* Settings::getWifiPassword(){
  return settingsInstance.wifiPassword;
}
char* Settings::getWifiSSID(){
  return settingsInstance.wifiSSID;
}

void Settings::setWifiSSID(char* ssid){
  strcpy(settingsInstance.wifiSSID,ssid);
  writeEEPROMSettings();
}
void Settings::setWifiPassword(char* psw){
  strcpy(settingsInstance.wifiPassword,psw);
  writeEEPROMSettings();
}

bool Settings::getEEPROMSettings(){
  EEPROM.get(0,settingsInstance);
  return true;
}

void Settings::writeEEPROMSettings(){
  EEPROM.put(0,settingsInstance);
}
