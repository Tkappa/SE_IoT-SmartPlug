void setup(){
  Serial.begin(115200);
}

int currcase=0;
String readString;

void loop(){
  switch(currcase){
    case 0:
    isConnected();
    break;
    case 1:
    break;
    case 2:
    break;
  }
  delay(100);
}


bool isConnected(){
   while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      if (c=='\n'){
        Serial.println(readString);
        readString="";
      }
      readString += c; //makes the string readString
    } 
  }
  return false;
}
void setupconn(){
  

}

void get(){}

