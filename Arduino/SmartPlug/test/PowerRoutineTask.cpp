
#include "PowerRoutineTask.h"

PowerRoutine::PowerRoutine(PowerRelay * inputDevice){
  currentDevice=inputDevice;
  currentTime=0;
  state=POW_hasTime;
}

void PowerRoutine::init(int inbasePeriod){
  Task::init(inbasePeriod);
  basePeriod=inbasePeriod;
}

void PowerRoutine::tick(){
  int arrayIndex;
switch (state) {
  case POW_hasTime:
  if(Flags::getInstance()->getHasTime()){
    currentTime=Flags::getInstance()->getSetupTime();
  }
  else{
    if(Flags::getInstance()->getMasterOnOff()){
      currentDevice->powerOn();
      Flags::getInstance()->setPowLedCommand(on);
    }
    else{
      currentDevice->powerOff();
      Flags::getInstance()->setPowLedCommand(off);
    }
  }
  break;
  case POW_clock:
    //Modulo a day in seconds
    currentTime=(currentTime+(basePeriod/1000))%(24*60*60);
    arrayIndex = currentTime/(MINUTESEVERYROUTINEINDEX*60);
    if(!Flags::getInstance()->getMasterOnOff()){
      currentDevice->powerOff();
      Flags::getInstance()->setPowLedCommand(off);
    }
    else{
      if(Settings::getInstance()->getRoutinePos(arrayIndex)){
        currentDevice->powerOn();
        Flags::getInstance()->setPowLedCommand(on);
      }
      else{
        currentDevice->powerOff();
        Flags::getInstance()->setPowLedCommand(off);
      }
    }
  break;
}
}
