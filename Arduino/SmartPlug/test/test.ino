#include "Timer.h"
#include "DeviceSettings.h"
#include "BluetoothInitTask.h"
#include "BluetoothRoutineTask.h"
#include "SoftwareSerial.h"
#include "WifiESP8266.h"
#include "WifiRoutineTask.h"
#include "BTHC05.h"

#include "DeviceFlags.h"

#define DEBUG

#define BTTX 2 //La porta TX che esce dal device BT
#define BTRX 3 //La porta RX che esce dal device BT
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
BTHC05 * btDev=new BTHC05(BTTX,BTRX,BTVCC,BTATMODE,BTSTATUS);
BluetoothInit btInitRout(btDev);
BluetoothRoutine btMsgRoutine(btDev);
//WifiESP8266 * wifiDev = new WifiESP8266();
//WifiRoutine wifiRout(WFTX,WFRX,wifiDev,4800);


const byte basePeriod=50;
Timer timer;


void setup(){
timer.setupPeriod(basePeriod);
 Serial.begin(9600);
 while (!Serial) {};
//btDev->btChannel= new SoftwareSerial(BTTX,BTRX);
 btDev->begin(9600);
 //btDev->btChannel= new SoftwareSerial(BTTX,BTRX);
 //btDev->btChannel->begin(9600);
 //caccamerda->begin(9600);
 //println("helomyfrine");



 btInitRout.init(1000);
 btMsgRoutine.init(1000);
 //wifiRout.init(1500);
 pinMode(BTBUTTON, INPUT);
 pinMode(BTLED,OUTPUT);
 Flags::getInstance()->setWFhasSettings(true);
 Settings::getInstance()->setWifiSSID("Almawifi per poveri");
 Settings::getInstance()->setWifiPassword("SanVitale45");
 Serial.println(freeRam());



}
void loop(){
  timer.waitForNextTick();
  //Serial.println(millis()-lastmillis);
  lastmillis=millis();

  if(btInitRout.updateAndCheckTime(basePeriod)){
    btInitRout.tick();

  }
  if(btMsgRoutine.updateAndCheckTime(basePeriod)){
    btMsgRoutine.tick();
    #ifdef DEBUG
      Serial.print(F("Ram remaining: "));
      Serial.println(freeRam());
    #endif
  }
  //if(wifiRout.updateAndCheckTime(basePeriod)){
  //  Serial.print("hmm");
    //wifiRout.tick();
  //}


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
  }
}
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
