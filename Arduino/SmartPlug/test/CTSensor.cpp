#include "CTSensor.h"

CTSensor::CTSensor(int AnalogPin){
  ticks=0;
  currentValueAmpere=0.0;
  currWeigth=0.0;
  pin= AnalogPin;
}

void CTSensor::begin(){
  emon.current(pin,CONSTCALIBRATION);
}


void CTSensor::read(){
  Flags::getInstance()->setValueRead(123.456);
}
/*  double Irms = emon.calcIrms(1480);
  double a= 0.1;
  currentValueAmpere= ((a*Irms)-weigth)+(1-a)*currentValueAmpere;
  Serial.println(currentValueAmpere);
  if(currentValueAmpere<0){
    weigth-=0.05;
  }


}*/

bool CTSensor::weigth(){
  return true;
}
/*  read();
  if(currentValueAmpere>0&&currentValueAmpere>0.5){
    if(ticks++>=20){
      ticks=0;
      currWeigth+=0.1;
      Serial.print("Adding weight: ");
      Serial.println(currWeigth);
    }
    return false;
  }
  else{
    Serial.println("TRUE");
    return true;
  }
}
*/
