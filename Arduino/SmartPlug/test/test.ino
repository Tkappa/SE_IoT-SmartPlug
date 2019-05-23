
#include "Timer.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

#include "DeviceSettings.h"
#include "DeviceFlags.h"

#include "BluetoothInitTask.h"
#include "BluetoothRoutineTask.h"
#include "DeviceRoutineTask.h"
#include "UXRoutineTask.h"
#include "PowerRoutineTask.h"

#include "WifiESP8266.h"
#include "WifiRoutineTask.h"
#include "BTHC05.h"
#include "CTSensor.h"
#include "LedImpl.h"
#include "KYPowRelay.h"
#include "ButtonImpl.h"


#define DEBUG

#define CTANALOGPIN 1

#define RELAY 1
#define BTTX 2 //La porta TX che esce dal device BT
#define BTRX 3 //La porta RX che esce dal device BT
#define BTVCC 4
#define BTATMODE 5
#define BTBUTTONPIN 6
#define BTSTATUS 7
#define WFRX 8 //La porta Rx che esce dal device WIFI
#define WFTX 9 //La porta Tx che esce dal device WIFI

#define DEVLEDPIN 10
#define BTLEDPIN 11
#define WIFILEDPIN 12
#define POWLEDPIN 13

int lastmillis=0;
bool pressedLastTick=false;
int ledState=LOW;

LedImpl * btLed = new LedImpl(BTLEDPIN);
LedImpl * wifiLed = new LedImpl(WIFILEDPIN);
LedImpl * devLed = new LedImpl(DEVLEDPIN);
LedImpl * powLed = new LedImpl(POWLEDPIN);
BtnImpl * btButtn = new BtnImpl(BTBUTTONPIN);

BTHC05 * btDev=new BTHC05(BTRX,BTTX,BTVCC,BTATMODE,BTSTATUS);
WifiESP8266 * wifiDev = new WifiESP8266(WFRX,WFTX);
CTSensor * wtgDev=new CTSensor(CTANALOGPIN);
KYPowRelay * KYPow= new KYPowRelay(RELAY);

BluetoothInit btInitRout(btDev);
BluetoothRoutine btMsgRoutine(btDev);
WifiRoutine wifiRout(wifiDev);
DeviceRout devRout(wtgDev);
PowerRoutine powRout(KYPow);
UXRoutine uxRout(btLed,wifiLed,devLed,powLed,btButtn);

const byte basePeriod=50;
Timer timer;


void setup(){
timer.setupPeriod(basePeriod);
 Serial.begin(9600);
 while (!Serial) {};

 btDev->begin(9600);
 wifiDev->begin(4800);
 wtgDev->begin();
 btLed->begin();
 wifiLed->begin();
 devLed->begin();
 powLed->begin();
 btButtn->begin();
 KYPow->begin();


 btInitRout.init(1000);
 btMsgRoutine.init(1000);
 wifiRout.init(500);
 powRout.init(1000);

 uxRout.init(50);
 devRout.init(50);

 Serial.println(freeRam());

}
void loop(){
  timer.waitForNextTick();
  //Serial.println(millis()-lastmillis);
  lastmillis=millis();
  if(devRout.updateAndCheckTime(basePeriod)){
    devRout.tick();
  }
  if(Flags::getInstance()->getDeviceReady()){
    if(Flags::getInstance()->getBTBtnRequest()){
        /*if(btInitRout.updateAndCheckTime(basePeriod)){
          btInitRout.tick();
        }
        if(btMsgRoutine.updateAndCheckTime(basePeriod)){
          btMsgRoutine.tick();
        }*/
     }
    else{
      /*if(wifiRout.updateAndCheckTime(basePeriod)){
        //Serial.print("hmm");
        wifiRout.tick();
        #ifdef DEBUG
          Serial.print(F("Ram remaining: "));
          Serial.println(freeRam());
        #endif
      }*/
    }
    /*if(powRout.updateAndCheckTime(basePeriod)){
      powRout.tick();
    }
    if(uxRout.updateAndCheckTime(basePeriod)){
      uxRout.tick();
    }*/
  }
}


int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
