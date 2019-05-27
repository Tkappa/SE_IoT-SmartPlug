#include "CTSensor.h"

CTSensor::CTSensor(int AnalogPin){
  ticks=0;
  currentValueAmpere=0.0;
  currWeigth=0.0;
  pin= AnalogPin;
}

void CTSensor::begin(){
  emon.current(pin,60);
}

void CTSensor::calculateWeigth(){
  if(currentValueAmpere>0&&currentValueAmpere>0.05){
    if(ticks++>=CTWEIGTHTICKS){
      ticks=0;
      currWeigth+=0.02;
      Serial.print("Adding weight: ");
      Serial.println(currWeigth);
    }
  }
}

void CTSensor::read(){
  //float valueread = 123.456;
//}
  //if(!Flags::getInstance()->getMasterOnOff()){
  //  calculateWeigth();
  //}
  double Irms = emon.calcIrms(200);
  Irms=Irms-currWeigth;
  //Serial.println(Irms);
  double a= 0.2;
  currentValueAmpere= ((a*Irms))+(1-a)*currentValueAmpere;
  Serial.println(currentValueAmpere);
  currentValueWattage=(currentValueAmpere*CONSTVOLTAGE)-WATTAGEWEIGTH ;
  if(currentValueWattage<0){
    currentValueWattage=0;
  }
  Serial.println(currentValueWattage);

  //Flags::getInstance()->setValueRead(currentValueWattage);

  Serial.println("--");
  if(currentValueAmpere<0){
    currWeigth-=0.01;
  }
}

bool CTSensor::weigth(){
  //return true;
  read();
  calculateWeigth();
  if(currentValueAmpere>0&&currentValueAmpere>0.05){
    return false;
  }
  else{
    return true;
  }
}
