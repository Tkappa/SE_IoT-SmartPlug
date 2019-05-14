#define DEBUG


#include "Timer.h"
#include "DeviceSettings.h"
#include "BluetoothInitTask.h"
#include "BluetoothRoutineTask.h"
#include "SoftwareSerial.h"
#include "WifiESP8266.h"
#include "WifiRoutineTask.h"

#include "DeviceFlags.h"

#define BTTX 2
#define BTRX 3
#define BTVCC 4
#define BTATMODE 5
#define BTBUTTON 6
#define BTSTATUS 7
#define WFRX 8
#define WFTX 9

#define BTLED 11

int lastmillis=0;
bool pressedLastTick=false;
int ledState=LOW;
//BluetoothInit test(BTTX,BTRX,BTVCC,BTATMODE,9600);
//BluetoothRoutine rout(BTTX,BTRX,BTSTATUS,9600);
//WifiESP8266 * wifiDev = new WifiESP8266();
//WifiRoutine wifiRout(WFTX,WFRX,wifiDev,4800);


const byte basePeriod=50;
Timer timer;


void setup(){
timer.setupPeriod(basePeriod);
 Serial.begin(9600);
 while (!Serial) {};
 
 //test.init(1000);
 //rout.init(1000);
 //wifiRout.init(1500);
 pinMode(BTBUTTON, INPUT);
 pinMode(BTLED,OUTPUT);
 //Flags::getInstance()->setWFhasSettings(true);
 //Settings::getInstance();//->setWifiSSID("Almawifi per poveri");
 //Settings::getInstance()->setWifiPassword("SanVitale45");
 Serial.println(freeRam());



}
void loop(){
  timer.waitForNextTick();
  //Serial.println(millis()-lastmillis);
  lastmillis=millis();

  //if(test.updateAndCheckTime(basePeriod)){
  //  Serial.print("hmm");
  //  test.tick();
  //}
  //if(rout.updateAndCheckTime(basePeriod)){
  //  Serial.print("hmm");
  //  rout.tick();
  //}
  //if(wifiRout.updateAndCheckTime(basePeriod)){
  //  Serial.print("hmm");
    //wifiRout.tick();
  //}
  
/*
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
    if(ledState==HIGH){
    digitalWrite(BTLED,LOW);
      ledState=LOW;
    }
    else{
      digitalWrite(BTLED,HIGH);
      ledState=HIGH;
    }
  }
  if(Flags::getInstance()->getBTLedCommand()==on){
    digitalWrite(BTLED,HIGH);
    ledState=HIGH;
  }
  if(Flags::getInstance()->getBTLedCommand()==off){
    digitalWrite(BTLED,LOW);
    ledState=LOW;
  }*/
  //while(test.available()){
  //  Serial.write(test.read());
  //}
}
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
