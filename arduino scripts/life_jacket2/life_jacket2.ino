
/*
* 
* testing script
* Data are displayed on 5 liquid crystal displays
* contrechoc 2013
*
*
*/

#include <PortsLCD.h>
#include <LifeJacket.h>

PortI2C myI2C3 (5);
PortI2C myI2C4 (6);
LiquidCrystalI2C lcd1 (myI2C3, 0x24);
LiquidCrystalI2C lcd2 (myI2C3, 0x26);
LiquidCrystalI2C lcd3 (myI2C3, 0x27);

LiquidCrystalI2C lcd4 (myI2C4, 0x24);
LiquidCrystalI2C lcd5 (myI2C4, 0x26);

LiquidCrystalI2C lcdArray[] = {   lcd1, lcd2, lcd3, lcd4, lcd5 };

char* lineOfText0 = (char *) malloc ( 20 * sizeof (char) ) ;
char* lineOfText1 = (char *) malloc ( 20 * sizeof (char) ) ;

unsigned char nextLine = 0;
unsigned char mySwitch = 0;

unsigned char testCounter = 0;
//01234567890123456   0    5    0    5    0
//0-10


//1234567890123456789012345678901234567890



  unsigned char  charCountArray[5];

void setup() {
  // set up the LCD's number of rows and columns: 
  delay(100);
  
  unsigned char  lcdCounter = 0;
  for ( lcdCounter=0;lcdCounter<5;lcdCounter++)
    lcdArray[lcdCounter].begin(16, 2);

  charCountArray[0] = 0;
  charCountArray[1] = 0;
  charCountArray[2] = 0;
  charCountArray[3] = 0;
  charCountArray[4] = 0;

  Serial.begin(9600);

  //pinMode(3, OUTPUT);
  //analogWrite( 3, 55);
}

int countThisChars( char *myCharArray ){
  unsigned char  counter = 0;
  while ( *myCharArray++ ) counter++;
  return counter;
}

void makeTextForLCDNumber( int lcdNumber, int lineNumber){
  //unsigned char  num = 0;
  // for (num = 0; num<5;num++){
 
    charCountArray[lcdNumber] = countThisChars( lifeJacket.linesArray[lcdNumber][lineNumber] );
    //Serial.println( charCountArray[num] );
    shiftTextOnLCD(  lcdNumber,   lineNumber);
  // }
}

void shiftTextOnLCD(int num, int lineNumber){
  // we have char0Count chars in text
  // we have a screen of 32

  unsigned char counter = 0;
  unsigned char length = charCountArray[num];
  while ( counter < 20 ){
    lineOfText0[counter] =  lifeJacket.linesArray[num][lineNumber][counter];
    counter++;
  }

  counter = 20;

  while ( (counter < 35)  ){
  lineOfText1[counter - 20] = lifeJacket.linesArray[num][lineNumber][counter];
   counter++;
  }
  
  lcdArray[num].clear();
  lcdArray[num].setCursor(0, 0);
  lcdArray[num].print(lineOfText0);
  lcdArray[num].setCursor(0, 1);
  lcdArray[num].print(lineOfText1);
  
  
  //Serial.println( lineOfText0 );
  //Serial.println( lineOfText1 );

}

void scrollAllLeft(){
  unsigned char i;
  unsigned char lcdCounter;
  for(i=0;i<5;i++){
    for ( lcdCounter=0;lcdCounter<5;lcdCounter++)
      lcdArray[lcdCounter].scrollDisplayLeft();
    delay(1000);
    //setMySwitch();
  }
}
/*
void setMySwitch(){
  if ( mySwitch == 0 ) { 
    //analogWrite( 3,   55 );
    mySwitch = 1;
  }
  else
  {
    //analogWrite( 3,   200 );
    mySwitch = 0;
  }
}
*/
void scrollAllRight(){
  unsigned char i;
  unsigned char lcdCounter;
  for(i=0;i<5;i++){
    for ( lcdCounter=0;lcdCounter<5;lcdCounter++)
      lcdArray[lcdCounter].scrollDisplayRight();
    delay(1000);
    //setMySwitch();
  }
}

void loop() {


 makeTextForLCDNumber( 0, 1+(( analogRead(5)/150 )%10 )  );
  makeTextForLCDNumber( 1, 1+((analogRead(4)/150 )%10 )  );
    makeTextForLCDNumber( 2, 1+((analogRead(3)/150 )%10)  );
    makeTextForLCDNumber( 3, 1+((analogRead(2)/150 )%10)    );
  makeTextForLCDNumber( 4, 1+((analogRead(1)/150 )%10) );
      
     // Serial.print(analogRead(2));  Serial.print(" \n ");
      /*
          Serial.print(analogRead(5));   Serial.print(" \t ");
     Serial.print(analogRead(4));  Serial.print(" \t ");
      Serial.print(analogRead(3));  Serial.print(" \t ");
       Serial.print(analogRead(2));  Serial.print(" \t ");
        Serial.println(analogRead(1)); 
*/

  //shiftTextOnLCD(0, nextLine % 10  );
  /*
   delay(1500);

   for(int i=0;i<5;i++){
   lcdArray[i].setCursor(0, 0);
  lcdArray[i].print("hello world");
  lcdArray[i].setCursor(0, 1);
  lcdArray[i].print("nice to meet");
   }

  delay(500);
   for(int i=0;i<5;i++){
    lcdArray[i].setCursor(0, 0);
  lcdArray[i].print("life jacket");
  lcdArray[i].setCursor(0, 1);
  lcdArray[i].print("save our souls");
   }
   */
    delay(1000);
}


