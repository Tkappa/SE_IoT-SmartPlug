#include "CTSensor.h"

CTSensor::CTSensor(int AnalogPin){
  ticks=0;
  currentValueAmpere=0.0;
  currweight=0.0;
  pin= AnalogPin;
}

void CTSensor::begin(){
  emon.current(pin,CONSTCALIBRATION);
}

void CTSensor::calculateweight(){
  if(currentValueAmpere>0&&currentValueAmpere>0.05){
    if(ticks++>=CTWEIGHTTICKS){
      ticks=0;
      currweight+=0.01;
      Serial.print("Adding weight: ");
      Serial.println(currweight);
    }
  }
}

void CTSensor::read(){

  //When the device is off we know that the reading should be 0 so we weight it further
  if(!Flags::getInstance()->getMasterOnOff()){
    calculateweight();
  }
  double Irms = emon.calcIrms(50);
  Irms=Irms-currweight;

  double a= 0.1;
  currentValueAmpere= ((a*Irms))+(1-a)*currentValueAmpere;
  currentValueWattage=(currentValueAmpere*CONSTVOLTAGE)-WATTAGEWEIGHT ;
  if(currentValueWattage<0){
    currentValueWattage=0;
  }

  #ifdef CTDEBUG
    Serial.print("New reading:")
    Serial.println(Irms);
    Serial.print("Median reading:")
    Serial.println(currentValueAmpere);
    Serial.print("Current Wattage:")
    Serial.println(currentValueWattage);
    Serial.println("--");
  #endif

  Flags::getInstance()->setValueRead(currentValueWattage);

  //If the median reading is less than zero then the weight is too strong
  if(currentValueAmpere<0){
    currweight-=0.01;
  }
}

bool CTSensor::weight(){
  read();
  calculateweight();
  if(currentValueAmpere>0&&currentValueAmpere>0.05){
    return false;
  }
  else{
    return true;
  }
}
