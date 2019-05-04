#include <SoftwareSerial.h>


const byte redPin=9;
const byte bluePin=10;
const byte greenPin=11;

const byte rxPin = 2;
const byte txPin = 3;

int redVal=0;
int blueVal=0;
int greenVal=0;

char target='S';
int finiteState=0;
String buff="";

bool getpost=false;

SoftwareSerial ESP8266 (rxPin, txPin);

unsigned long lastTimeMillis = 0;

void setup() {
  Serial.begin(4800); 
  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin,OUTPUT); 
  ESP8266.begin(4800);
  delay(2000);
}

void printResponse() {
  while (ESP8266.available()) {
    Serial.println(ESP8266.readStringUntil('\n')); 
  }
}

void resetFiniteState(){
target='S';
finiteState=0;
buff="";
  
}

void loop() {
  if (millis() - lastTimeMillis > 20000) {
    lastTimeMillis = millis();
    if (getpost==false){
      ESP8266.println("AT+CIPMUX=1");
      delay(1000);
      printResponse();
  
      ESP8266.println("AT+CIPSTART=0,\"TCP\",\"192.168.1.62\",8000");
      delay(2000);
      printResponse();
  
      String cmd = "GET / HTTP/1.1";
      ESP8266.println("AT+CIPSEND=0," + String(cmd.length() + 4));
      delay(2000);
  
      ESP8266.println(cmd);
      delay(1000);
      ESP8266.println("");
      getpost=true;
    }
    else{
      ESP8266.println("AT+CIPMUX=1");
      delay(1000);
      printResponse();
  
      ESP8266.println("AT+CIPSTART=0,\"TCP\",\"192.168.1.62\",8000");
      delay(2000);
      printResponse();
  
      String cd = "POST /hi HTTP/1.1";
      String cm = "Host: 192.168.1.62:8000";
      String mm = "Accept: *//*";
      String md = "Content-Type: application/x-www-form-urlencoded";
      String data = "value=5";
      
      String dd = "Content-Length:" + String(data.length());
      
      
      ESP8266.println("AT+CIPSEND=0," + String(cd.length()+ 2 + cm.length() +2+md.length() +2+mm.length() +2+data.length()+ 2+dd.length() + 4));
      delay(2000);
  
      ESP8266.println(cd);
      delay(200);
      ESP8266.println(cm);
      delay(200);
      ESP8266.println(mm);
      delay(200);
      ESP8266.println(md);
      delay(200);
      ESP8266.println(dd);
      delay(200);
      ESP8266.println("");
      delay(200);
      ESP8266.println(data);
      delay(200);
      ESP8266.println("");
      
      getpost=false;
    }
  }
  int len=0;
  len=ESP8266.available();
  if (len>0) {
    while(ESP8266.available()){
      char c=ESP8266.read();
      Serial.write(c);
      switch(finiteState){
      case 0:
        if(c==target){
          finiteState=1;
          target='P';
          Serial.print("-0-");
        }
        else{
          resetFiniteState();
        }
      break;
      case 1:
        if(c==target){
          finiteState=2;
          target='+';
          Serial.print("-1-");
        }
        else{
          resetFiniteState();
        }
      break;
      case 2:
      if(c==target){
          finiteState=3;
          target=';';
          Serial.print("-2-");
        }
        else{
          resetFiniteState();
        }
      break;
      case 3:
      if(c==target){
        finiteState=4;
        target=';';
        redVal=buff.toInt();
        buff="";
        Serial.print("-3, val is ");
        Serial.print(redVal);
         Serial.print("-");
      }
      else{
        buff.concat(c);
        
        Serial.print("-"+buff+"-");
      }
      
      break;
      case 4:
        if(c==target){
        finiteState=5;
        target=';';
        blueVal=buff.toInt();
        buff="";
        Serial.print("-3, val is ");
        Serial.print(blueVal);
         Serial.print("-");
      }
      else{
        buff.concat(c);
        
        Serial.print("-"+buff+"-");
      }
      break;
      case 5:
      if(c==target){
        finiteState=6;
        target='-';
        greenVal=buff.toInt();
        buff="";
        Serial.print("-3, val is ");
        Serial.print(greenVal);
         Serial.print("-");
      }
      else{
        buff.concat(c);
        
        Serial.print("-"+buff+"-");
      }
      break;
      case 6:
      if(c==target){
        finiteState=7;
        target='E';
        Serial.print("-REICIEVED-");
         Serial.print("-");
      }
      else{
        resetFiniteState();
      }
      break;
      case 7:
      resetFiniteState();
      break;
      default:
      break;
      }
      
      
    }
  }
  analogWrite(redPin,redVal);
  analogWrite(bluePin,blueVal);
  analogWrite(greenPin,greenVal);
  
}
