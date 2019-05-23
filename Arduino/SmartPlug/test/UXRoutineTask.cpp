#include "UXRoutineTask.h"

UXRoutine::UXRoutine(Led * inBtLed,Led * inWifiLed, Led * inDeviceLed, Led * inPowerLed, Button * inBTButton){
  BtLed=inBtLed;
  WifiLed=inWifiLed;
  DeviceLed=inDeviceLed;
  PowerLed=inPowerLed;
  BTButton=inBTButton;

  BtLedflash=0;
  WifiLedflash=0;
  DeviceLedflash=0;
  PowerLedflash=0;
  buttonPressedlastTick=false;
}

  void UXRoutine::init(int basePeriod){
    Task::init(basePeriod);
    BtLedflash=0;
    WifiLedflash=0;
    DeviceLedflash=0;
    PowerLedflash=0;
    buttonPressedlastTick=false;
  }
  void UXRoutine::tick(){

    //Check button
    int state = digitalRead(BTButton);
    if (BTButton->isPressed()){
      if(buttonPressedlastTick){
        Flags::getInstance()->setBTBtnRequest(true);
      }
      else{
        buttonPressedlastTick=true;
      }
    }
    else{
      buttonPressedlastTick=false;
    }


    //Handle BT Led
    if(Flags::getInstance()->getBTLedCommand()==flashing){
      BtLedflash++;
      if(BtLedflash>=FLASHINGTICKLEN){
        BtLedflash=0;
        BtLed->toggle();
      }
    }
    if(Flags::getInstance()->getBTLedCommand()==on){
      BtLed->turnOn();
    }
    if(Flags::getInstance()->getBTLedCommand()==off){
      BtLed->turnOff();
    }


    //Handle wifiLed
    if(Flags::getInstance()->getWifiLedCommand()==flashing){
      WifiLedflash++;
      if(WifiLedflash>=FLASHINGTICKLEN){
        WifiLedflash=0;
        WifiLed->toggle();
      }
    }
    if(Flags::getInstance()->getWifiLedCommand()==on){
      WifiLed->turnOn();
    }
    if(Flags::getInstance()->getWifiLedCommand()==off){
      WifiLed->turnOff();
    }

    //Handle DeviceLed
    if(Flags::getInstance()->getDevLedCommand()==flashing){
      DeviceLedflash++;
      if(DeviceLedflash>=FLASHINGTICKLEN){
        DeviceLedflash=0;
        BtLed->toggle();
      }
    }
    if(Flags::getInstance()->getDevLedCommand()==on){
      DeviceLed->turnOn();
    }
    if(Flags::getInstance()->getDevLedCommand()==off){
      DeviceLed->turnOff();
    }

    //HandlePowerled
    if(Flags::getInstance()->getPowLedCommand()==flashing){
      PowerLedflash++;
      if(PowerLedflash>=FLASHINGTICKLEN){
        PowerLedflash=0;
        PowerLed->toggle();
      }
    }
    if(Flags::getInstance()->getPowLedCommand()==on){
      PowerLed->turnOn();
    }
    if(Flags::getInstance()->getPowLedCommand()==off){
      PowerLed->turnOff();
    }

  }
