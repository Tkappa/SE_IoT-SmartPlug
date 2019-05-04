#define BUTTON_PIN 2
#define CURRENT_PIN 12
#define LED_PIN 8

#include "EmonLib.h"
#include "TimerOne.h"
// Include Emon Library
EnergyMonitor emon1;
// Create an instance


bool switched = false;
bool currstatus= false;
int maxWattage = 80;
int strikes = 5;
int currstrikes=0;
bool Routine[10]={1,0,1,0,1,1,0,1,0,0};
bool input[]={1,0,1,0,0,0,0,1,0,0};
int RoutineClockLengthinS=10;
bool readvalue=false;
int arrayPos=0;

void setup() {
 
 //Routine=input;
 pinMode(CURRENT_PIN, OUTPUT);
 pinMode(BUTTON_PIN, INPUT);
 pinMode(LED_PIN,OUTPUT);
 Serial.begin(9600);
 emon1.current(1, 111.1);  
 Timer1.initialize(RoutineClockLengthinS*1000000);
 Timer1.attachInterrupt(readValue);
}
void loop() {
 int buttonState = digitalRead(BUTTON_PIN);
 if (buttonState == HIGH && !switched) {
   switched = true;
   currstatus = !currstatus;
   digitalWrite(LED_PIN, currstatus);
 } else {
   if(buttonState==LOW)  switched = false;
 }
 double Irms = emon1.calcIrms(1480);  // 
 double Wattage= Irms*230.0;
 
 if(Wattage>maxWattage){
   currstrikes++;
   if(currstrikes>strikes){
   Serial.println("Salvavita!");
   currstatus = false;
   digitalWrite(LED_PIN, currstatus);
   currstrikes=0;
   }
 }
 else{
   currstrikes=0;
 }
  Serial.print("Current Wattage: ");
  Serial.println((Irms*230.0));
  if(currstatus){
    digitalWrite(CURRENT_PIN, readvalue);
  }
  else{
    digitalWrite(CURRENT_PIN, LOW);
  }
}

void readValue(){
  Serial.print("Reading value: ");
  readvalue = Routine[arrayPos];
  Serial.println(readvalue);
  arrayPos = (arrayPos+1)%10;
}


