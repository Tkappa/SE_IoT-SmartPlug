
int pinOut = 8;

void setup(){
  
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  
}

void loop()
{
 
  delay(5000);  
  Serial.println("high");
  digitalWrite(pinOut, HIGH);
  delay(5000);
  Serial.println("low");
  digitalWrite(pinOut, LOW);
  
}

