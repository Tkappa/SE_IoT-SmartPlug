#include "WifiTask.h"


WifiTask::WifiTask(int tx,int rx,WifiDevice * inputDevice){
  txPin=tx;
  rxPin=rx;
  wifiChannel= new SoftwareSerial(tx, rx);
  currentDevice = inputDevice;

}

void WifiTask::begin(long baud){
  wifiChannel->begin(baud);
  baudRate=baud;
}

void WifiTask::write(uint8_t input){
  wifiChannel->write(input);
}

int WifiTask::read(){
  return wifiChannel->read();
}

int WifiTask::available(){
  return wifiChannel->available();
}
