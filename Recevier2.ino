#include <SPI.h>
#include <LoRa.h>
int sensorState1 = 0;
int sensorState2 = 0;
#define ss 10
#define rst 9
#define dio0 2
int i=0;
int Start_Stop = 4;                                                   
int Speed = 5;                                                       
int Direction = 6;
int Led = 3;
int voltage;
int offset = 0;
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  pinMode(Led, OUTPUT);
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
    LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {
  serialEvent();
  if (stringComplete) {
    Serial.println(inputString);
    if (inputString=="Led On\n"){
      digitalWrite(Led, HIGH);
      Motor();
      delay(200);
      VoltageCheck();
      delay(200);
      Percentage();
      delay(200);
      DirectionCheck();
      delay(200);
          i=1;
         atmus();
    }

    else if (inputString=="Led Off\n"){
      digitalWrite(Led, LOW);
    }

    inputString = "";
    stringComplete = false;
  } 
}

void serialEvent() {

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      char inChar = (char)LoRa.read();
      inputString += inChar;
      if (inChar == '\n') {
        stringComplete = true;
      
    }
    }
  }
}

 void Motor(){
      LoRa.beginPacket();
      LoRa.print("Motor is on\n");
      LoRa.endPacket();
  }
  
void VoltageCheck(){
  int volt = analogRead(35);
  voltage = map(volt, 0, 4095, 0, 2500) + offset;
  voltage /= 100;
  LoRa.beginPacket();
  LoRa.print("Volt= ");
  LoRa.print(voltage);
    LoRa.print("\n");
   LoRa.endPacket();
   }

void Percentage(){
     LoRa.beginPacket();
    if(voltage <= 25 && voltage >=24){
    LoRa.print("Battery=100%\n");}
    else if(voltage <= 24 && voltage >=23){
     LoRa.print("Battery=80%\n");}
    else if(voltage <= 23 && voltage >=22){
     LoRa.print("Battery=60%\n");}
    else if(voltage <= 22 && voltage >=21){
     LoRa.print("Battery=40n");}
     else if(voltage <= 21 && voltage >=20){
     LoRa.print("Battery=20%\n");}
     else if(voltage <= 20){
     LoRa.print("Battery=0%\n");}
       LoRa.endPacket();
    } 
void DirectionCheck(){
     LoRa.beginPacket(); 
     LoRa.print("Clockwise\n");
       LoRa.endPacket();
      }

 void atmus(){ 
   while(i)
    {
    relay_off();
  if(i==0)
   break;
   sensorState1 = digitalRead(7);
   sensorState2 = digitalRead(8);
   Serial.println(sensorState1);
   delay(100);
   Serial.println(sensorState2);

   
   if (sensorState1 == HIGH) {            //Sensor1 is activated anticlockwise
      digitalWrite(Start_Stop,LOW);
      delay(1000);
      digitalWrite(Speed,HIGH);
      digitalWrite(Direction,LOW);
      digitalWrite(Start_Stop,HIGH); 
       }
    
   else if (sensorState2 == HIGH){          //Sensor2 is activated clockwise
      digitalWrite(Start_Stop,LOW);
      delay(1000);
      digitalWrite(Speed,HIGH);
      digitalWrite(Direction,HIGH);
      digitalWrite(Start_Stop,HIGH);
    } 
  }
 }

 void relay_off(){  
  serialEvent(); 
  Serial.println(inputString);               
     if (inputString=="Led Off\n") {                                         
      digitalWrite(Led, LOW);  
      i=0;
      digitalWrite(Speed,LOW);
      digitalWrite(Direction,LOW);
      digitalWrite(Start_Stop,LOW);
        }
      inputString = "";
    stringComplete = false; 
  }
