#define DEBUG


#include "Timer.h"
#include "DeviceSettings.h"
#include "BluetoothInitTask.h"
#include "SoftwareSerial.h"

#include "DeviceFlags.h"

#define BTTX 2
#define BTRX 3
#define BTVCC 4
#define BTATMODE 5
#define BTBUTTON 6
#define BTLED 11

int lastmillis=0;
bool pressedLastTick=false;
BluetoothInit test(BTTX,BTRX,BTVCC,BTATMODE,9600);

const int basePeriod=50;
Timer timer;


void setup(){
  timer.setupPeriod(basePeriod);
 Serial.begin(9600);
 while (!Serial) {};
  
 test.init(1000);
 pinMode(BTBUTTON, INPUT); 
 pinMode(BTLED,OUTPUT);



}
void loop(){
  timer.waitForNextTick();
  //Serial.println(millis()-lastmillis);
  lastmillis=millis();
  
  if(test.updateAndCheckTime(basePeriod)){
    test.tick();
  }
  int state = digitalRead(BTBUTTON);
  if (state==HIGH){
    if(pressedLastTick){
      Flags::getInstance()->setBTBtnRequest(true);
    }
    else{
    pressedLastTick=true;
    }
  }
  else{
    pressedLastTick=false;
  }
  
  if(Flags::getInstance()->getBTLedCommand()==flashing){
    digitalWrite(BTLED,HIGH);
  }
  //while(test.available()){
  //  Serial.write(test.read());
  //}
}
