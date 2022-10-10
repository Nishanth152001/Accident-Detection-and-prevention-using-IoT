


#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

char receivedChar;// received value will be stored as CHAR in this variable

const int ML1 = 27; //ESP32 pins (MR=Right Motor) (ML=Left Motor) (1=Forward) (2=Backward)
const int ML2 = 26; 
int enable1PinA = 14; 
const int MR1 = 32;
const int MR2 = 35;
int enable1PinB = 13; 


const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;


void setup() {
  Serial.begin(115200);
  SerialBT.begin("Nishanth"); //You can change your Bluetooth device name
  pinMode(MR1, OUTPUT); 
  pinMode(MR2, OUTPUT);
  pinMode(enable1PinA, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(enable1PinB, OUTPUT);
   ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1PinA, pwmChannel);
  ledcAttachPin(enable1PinB, pwmChannel);

}

void Forward(){
        //RIGHT MOTOR
      digitalWrite(MR1,HIGH);//MOVE FRONT
      digitalWrite(MR2,LOW); //MOVE BACK
       ledcWrite(pwmChannel, 255); 
      //LEFT MOTOR
      digitalWrite(ML1,LOW);//MOVE BACK
      digitalWrite(ML2,HIGH);//MOVE FRONT
       ledcWrite(pwmChannel, 255); 
}
void Backward(){
      digitalWrite(MR1,LOW);
      digitalWrite(MR2,HIGH);
       ledcWrite(pwmChannel, 255); 
      digitalWrite(ML1,HIGH);
      digitalWrite(ML2,LOW);
       ledcWrite(pwmChannel, 255); 
}
void Left(){
      digitalWrite(MR1,HIGH);
      digitalWrite(MR2,LOW);
       ledcWrite(pwmChannel, 255); 
      digitalWrite(ML1,HIGH);
      digitalWrite(ML2,LOW);
       ledcWrite(pwmChannel, 255); 
}
void Right(){
      digitalWrite(MR1,LOW);
      digitalWrite(MR2,HIGH);
       ledcWrite(pwmChannel, 255); 
      digitalWrite(ML1,LOW);
      digitalWrite(ML2,HIGH);
       ledcWrite(pwmChannel, 255); 
}
void Stop(){
      digitalWrite(MR1,LOW); 
      digitalWrite(MR2,LOW);
       ledcWrite(pwmChannel, 255); 
      digitalWrite(ML1,LOW); 
      digitalWrite(ML2,LOW);
       ledcWrite(pwmChannel, 255);  
}
void loop() {
    receivedChar =(char)SerialBT.read();

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  
  }
  if (SerialBT.available()) {
     
    Serial.print ("Received:");//print on serial monitor
    Serial.println(receivedChar);//print on serial monitor    
    
    if(receivedChar == 'F')
    {
      Forward();
       
    }
    if(receivedChar == 'G')
    {
 
      Backward(); 
    }         
     if(receivedChar == 'L')
    {

      Left();
    }        
    if(receivedChar == 'R')
    {

      Right(); 
    }
    if(receivedChar == 'S')
    {
      Stop();
    }
  }
  delay(20);
}
 
