#include "EmonLib.h"
#include "CTSensor.h"
// Include Emon Library
EnergyMonitor emon1;
// Create an instance

bool statusS= false;
CTSensor ct(1);

void setup()
{
  Serial.begin(9600);
  ct.begin();
  //emon1.current(1, 60.9);             // Current: input pin, calibration.
}

void loop()
{
  if(!statusS){
    
    if(ct.weigth()){
      statusS=true;
    }
  }
  else{
    ct.read();
  }
  /*double Irms = emon1.calcIrms(2000);  // Calculate Irms only
  Serial.print((Irms*230.0));           // Apparent power
  Serial.print(" ");
  Serial.println(Irms);             // Irms*/
}
