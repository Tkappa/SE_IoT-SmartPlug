
#include "BluetoothTask.h"


BluetoothTask::BluetoothTask(int tx,int rx){
  txPin=tx;
  rxPin=rx;
  btChannel= new SoftwareSerial(tx, rx);
}

void BluetoothTask::begin(long baud){
  btChannel->begin(baud);
  baudRate=baud;
}

void BluetoothTask::write(uint8_t input){
  btChannel->write(input);
}

int BluetoothTask::read(){
  return btChannel->read();
}

int BluetoothTask::available(){
  return btChannel->available();
}
