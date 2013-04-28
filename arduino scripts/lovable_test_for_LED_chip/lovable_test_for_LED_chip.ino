/*
* test script to see if the data are having a response inthe LED's
* contrechoc 2013 NASA challenge
*
*
*/

#include <PortsLCD.h>
#include <LifeJacket.h>

#include <JeeLib.h>

char buf0[] = {    
  "exo*24 Sex b*0*1.99*3.333*452.8*1.333*0.09"       };
char buf1[] = {    
  "exo*CoRoT-3 b*0*21.66*1.01*4.2568*0.057*0.0,,,,85.9"       };
char buf2[] = {    
  "exo*HAT-P-13 b*0*0.85*1.28*2.916243*0.0426"       };
char buf3[] = {    
  "exo*HD 149026 b*0*0.356*0.718*2.8758916*0.04288"       };
char buf4[] = {    
  "exo*HD 87883 b*0*12.1**2754*3.6*0.53"       };

unsigned char countBuf = 0;


void setup() {
  // set up the LCD's number of rows and columns: 
 
  rf12_initialize('R', RF12_433MHZ, 212);
  Serial.begin(9600);
  
  pinMode (6, OUTPUT);
  digitalWrite(6,HIGH);
  delay(100);
   digitalWrite(6,LOW);
    delay(100);
    
      pinMode (16, OUTPUT);
  digitalWrite(16,HIGH);
  delay(100);
   digitalWrite(16,LOW);
    delay(100);
  
}

void loop(){
  
  
  while ((!rf12_canSend()) )
    rf12_recvDone();
if ( countBuf == 0 )  rf12_sendStart(0, buf0 , 30 , 0);
if ( countBuf == 1 )  rf12_sendStart(0, buf1 , 30 , 0);
if ( countBuf == 2 )  rf12_sendStart(0, buf2 , 30 , 0);
if ( countBuf == 3 )  rf12_sendStart(0, buf3 , 30 , 0);
if ( countBuf == 4 )  rf12_sendStart(0, buf4 , 30 , 0);
if ( countBuf == 5 )  rf12_sendStart(0, buf2 , 30 , 0);
countBuf++;
countBuf = countBuf%5;

  digitalWrite(6,HIGH);
  delay(100);
   digitalWrite(6,LOW);
    delay(300);
          pinMode (16, OUTPUT);
  digitalWrite(16,HIGH);
  delay(100);
   digitalWrite(16,LOW);
    delay(100);
    Serial.println("sentttt");
 
  
}
