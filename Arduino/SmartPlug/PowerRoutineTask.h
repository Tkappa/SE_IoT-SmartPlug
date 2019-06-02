#ifndef __POWROUTINE__
#define __POWROUTINE__

//How long does a routine index last for in minutes
#define MINUTESEVERYROUTINEINDEX 10

#include "PowerRelay.h"
#include "DeviceFlags.h"
#include "DeviceSettings.h"
#include "Task.h"
#include "Arduino.h"

enum POWER_RoutineStates{POW_hasTime,POW_clock};

class PowerRoutine:public Task{

  public:
    PowerRoutine(PowerRelay * inputDevice);

    void init(int inbasePeriod);
    void tick();


    protected:
    PowerRelay * currentDevice;
    POWER_RoutineStates state;
    int currentTime;
    int basePeriod;
};


#endif
