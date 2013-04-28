#include <JeeLib.h>


char inputString[] = "hello world!";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
unsigned char c = 0;
char buf7[] = {    "012345678901234567890123456789012345678901234567890123456789"       };
 
 long timer;
 
 unsigned char bufferLength = 40;
 
void setup(){
  
   rf12_initialize('R', RF12_433MHZ, 212);
   Serial.begin(9600);
   pinMode(7, OUTPUT);
     digitalWrite(7,HIGH);
  delay(100);
   digitalWrite(7,LOW);
   
 c =10;
  rf12_sendStart(0, inputString, c , 0);
  
}

void loop(){
  
  lookForSerial();
  
  if ( (0==1)&& (timer < millis())){
    
       digitalWrite(7,HIGH);
  delay(100);
   digitalWrite(7,LOW);
   timer = millis() + 10000;
   char buf8[] = {    "time45678901234567890"       };
      while ((!rf12_canSend()) )
    rf12_recvDone();

  rf12_sendStart(0, buf8 , 20 , 0);
  }
 
  delay(1);
  
}

void lookForSerial() {
  if ( Serial.available() ){
  
  Serial.readBytes(buf7, bufferLength) ;
 
   sendChars();
    Serial.flush();
    c=0;
       digitalWrite(7,HIGH);
  delay(100);
   digitalWrite(7,LOW);
  }
 
}
 
void sendChars(){
  
       while ((!rf12_canSend()) )
    rf12_recvDone();

  rf12_sendStart(0, buf7 , bufferLength , 0);
  
    delay(50);
 
 
  
}

