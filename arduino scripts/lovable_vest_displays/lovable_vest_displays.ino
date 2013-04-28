
#include <PortsLCD.h>
#include <LifeJacket.h>

#include <JeeLib.h>

PortI2C myI2C3 (5);
PortI2C myI2C4 (6);
LiquidCrystalI2C lcd1 (myI2C3, 0x24);
LiquidCrystalI2C lcd2 (myI2C3, 0x26);
LiquidCrystalI2C lcd3 (myI2C3, 0x27);

LiquidCrystalI2C lcd4 (myI2C4, 0x24);
LiquidCrystalI2C lcd5 (myI2C4, 0x26);

LiquidCrystalI2C lcdArray[] = {   
  lcd1, lcd3, lcd2, lcd4, lcd5 };

char* lineOfText0 = (char *) malloc ( 20 * sizeof (char) ) ;
char* lineOfText1 = (char *) malloc ( 20 * sizeof (char) ) ;

unsigned char nextLine = 0;
unsigned char mySwitch = 0;

unsigned char lcdLineNumber = 0;

unsigned char testCounter = 0;
//01234567890123456   0    5    0    5    0
//0-10


//1234567890123456789012345678901234567890

unsigned char fadePin = 5;    // LED connected to digital pin 9

unsigned char  charCountArray[5];

void setup() {
  // set up the LCD's number of rows and columns: 

  unsigned char  lcdCounter = 0;
  for ( lcdCounter=0;lcdCounter<5;lcdCounter++)
    lcdArray[lcdCounter].begin(16, 2);

  charCountArray[0] = 0;
  charCountArray[1] = 0;
  charCountArray[2] = 0;
  charCountArray[3] = 0;
  charCountArray[4] = 0;

  rf12_initialize('R', RF12_433MHZ, 212);
  Serial.begin(9600);

  for (unsigned char num=0;num<5;num++)
    displayOnCrystal( num++, "CoRoT-23 b - adopted", "...", "Beam", "Contrechoc");

  fade();


}

int countThisChars( char *myCharArray ){
  unsigned char  counter = 0;
  while ( *myCharArray++ ) counter++;
  return counter;
}





void displayOnCrystal(unsigned char num, String myCharArray1, String myCharArray2, String name1, String name2){
  lcdArray[num].clear();
  lcdArray[num].setCursor(0, 0);
  lcdArray[num].print(myCharArray1);
  lcdArray[num].setCursor(0, 1);
  lcdArray[num].print(name1);
   lcdArray[num].setCursor(8, 1);
  lcdArray[num].print(name2);
}



void loop() {

  char* buf = (char*) rf12_data;
  if (rf12_recvDone() && rf12_crc == 0) {
    unsigned char n = 0;
    unsigned char b = 0;
    unsigned char m = 0;
    unsigned char r = 0;
    unsigned char p = 0;
    unsigned char c = 0;
    unsigned char fn = 0;
    unsigned char ln = 0;
    String name = "          ";
    String binary = "  ";
    String mass = "     ";
    String radius = "     ";
    String period = "     ";
    String firstName = "            ";
    String lastName = "           ";

    for (int i = 0; i<30; i++){
      if ( buf[i]=='*') c++;
      if ( c==1 ) name[n++] = buf[i];
      if ( c==2 ) binary[b++] = buf[i];
      if ( c==3 ) mass[m++] = buf[i];
      if ( c==4 ) radius[r++] = buf[i];
      if ( c==5 ) period[p++] = buf[i];
      if ( c==6 ) period[fn++] = buf[i];
      if ( c==7 ) period[ln++] = buf[i];
    }
Serial.print( "1 " );
    Serial.println( name );Serial.print( "2 " );
    Serial.println( binary );Serial.print( "3 " );
    Serial.println( mass );Serial.print( "4 ");
    Serial.println( radius );Serial.print( "5 " );
    Serial.println( period );Serial.print( "6 ");
    Serial.println( firstName );Serial.print( "7 " );
    Serial.println( lastName );


    if ( checkMass(mass) ) {
      Serial.println("*-------------------ok");
      fade();
      for (int ii=0;ii<5;ii++){
        lcdArray[lcdLineNumber%5].noBacklight();
        delay(200);
        lcdArray[lcdLineNumber%5].backlight();
        delay(100);
      }

    }
    displayOnCrystal(lcdLineNumber%5, name, "adoption?", firstName, lastName);

    lcdLineNumber++;
    delay(100);
  }
  delay(1);

  //shiftTextOnLCD(int num, int lineNumber);

  delay(1000);
}

boolean checkMass(String mass){
  for (int i = 1; i<10; i++){
    if ( (mass[i]=='.') && ( mass[i-1]=='0'))
      return true;
  }
  return false;
}


void fade(){

  for(int fadeValue = 10 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(fadePin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(5);                            
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 10; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(fadePin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 

}





