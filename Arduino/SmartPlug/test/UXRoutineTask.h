#ifndef __UX__
#define __UX__

#include "Task.h"
#include "Led.h"
#include "Button.h"
#include "DeviceFlags.h"
#include "Arduino.h"

#define FLASHINGTICKLEN 4

class UXRoutine:public Task{

  public:
    UXRoutine(Led * inBtLed,Led * inWifiLed, Led * inDeviceLed, Led * inPowerLed, Button * inBTButton);

    void init(int inbasePeriod);
    void tick();

  protected:
    Led * BtLed;
    Led * WifiLed;
    Led * DeviceLed;
    Led * PowerLed;
    Button * BTButton;

    int BtLedflash;
    int WifiLedflash;
    int DeviceLedflash;
    int PowerLedflash;
    bool buttonPressedlastTick;
};
#endif
