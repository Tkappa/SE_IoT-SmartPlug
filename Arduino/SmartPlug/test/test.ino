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
#define WFRX 8 //La porta Rx che esce dal device WIFI
#define WFTX 9 //La porta Tx che esce dal device WIFI

#define BTLED 11

int lastmillis=0;
bool pressedLastTick=false;
int ledState=LOW;

//BTHC05 * btDev=new BTHC05(BTRX,BTTX,BTVCC,BTATMODE,BTSTATUS);
WifiESP8266 * wifiDev = new WifiESP8266(WFRX,WFTX);

//BluetoothInit btInitRout(btDev);
//BluetoothRoutine btMsgRoutine(btDev);
WifiRoutine wifiRout(wifiDev);


const byte basePeriod=50;
Timer timer;


void setup(){
timer.setupPeriod(basePeriod);
 Serial.begin(9600);
 while (!Serial) {};

 //btDev->begin(9600);
 wifiDev->begin(4800);

 //btInitRout.init(1000);
 //btMsgRoutine.init(1000);
 wifiRout.init(500);
 pinMode(BTBUTTON, INPUT);
 pinMode(BTLED,OUTPUT);
 Flags::getInstance()->setWFhasSettings(true);
 Settings::getInstance()->setWifiSSID("Almawifi per poveri");
 Settings::getInstance()->setWifiPassword("SanVitale45");
 Settings::getInstance()->setServerIp("192.168.1.63");
 Settings::getInstance()->setServerPort("8000");
 Flags::getInstance()->setValueRead(123.456);
 Settings::getInstance()->setSecretKey("1234");
 Settings::getInstance()->setID("01");
 Serial.println(freeRam());

}
void loop(){
  timer.waitForNextTick();
  //Serial.println(millis()-lastmillis);
  lastmillis=millis();
/*
  if(btInitRout.updateAndCheckTime(basePeriod)){
    btInitRout.tick();

  }
  if(btMsgRoutine.updateAndCheckTime(basePeriod)){
    btMsgRoutine.tick();
    
  }*/
  if(wifiRout.updateAndCheckTime(basePeriod)){
    //Serial.print("hmm");
    wifiRout.tick();
    #ifdef DEBUG
      Serial.print(F("Ram remaining: "));
      Serial.println(freeRam());
    #endif
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
