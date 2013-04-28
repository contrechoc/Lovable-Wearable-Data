 
//#include <RF12.h>
#include <Ports.h>
#include <JeeLib.h>
 
 
PortI2C myport (2 /*, PortI2C::KHZ400 */);
DeviceI2C rtc (myport, 0x68);

long timer;
long interval = 999;
 
 byte outBuf[350];
 byte countIR;
 byte countResend;
 
InfraredPlug ir (4);

unsigned char fadePin = 9;    // LED connected to digital pin 9


 word pos = 0;//to save the code

void resendIR(){
  
  if ( pos == 0 ){
   const byte* inBuf = ir.buffer();

        // don't decode the packet, just send it out again
        memset(outBuf, 0, sizeof outBuf);
       
        
        for (byte i = 0; i < countResend; ++i) {
            // determine how many slots IR was on or off
            byte nibble = (inBuf[i/2] >> (i % 2) * 4) & 0x0F;
            if (nibble > 10)
                nibble += nibble - 10;
            // nibble now contains number of slots
            
            for (byte j = 0; j < nibble; ++j) {
                if (i % 2 == 0) // even nibbles correspond to "on"
                    bitSet(outBuf[pos/8], pos%8);
                ++pos;
            }
        }
        
        Serial.print("Got code");
  }
  
   delay(3000);
   
        //Serial.print("Got ");
        //Serial.print(pos);
        //Serial.println(" bits");
            
        // send out IR codes three times at 1-second intervals
        for (byte n = 0; n < 1; ++n) {
           
            //ir.configure(140 - 5*(5 - n),80);
            ir.send(outBuf, pos);
           // Serial.print(140 - 5*(5 - n), DEC);
            Serial.println(" sent!");
        }
        
}


void setup()
{
   rf12_initialize('R', RF12_433MHZ, 212);

  Serial.begin(9600);
  Serial.println("start test");
}

void loop()
{
  
  //for testing
 
    if (Serial.available()) {
        char c = Serial.read();
          if( c != '1') Serial.print(c);delay(10);
          
          if( c == 'r'){
              Serial.print(c);delay(10);
             Serial.println("resend");
        
            resendIR();
          }
    }
 
 
    //---------------------------------ir scanning 
    ir.poll();
    countIR = 0;
    countIR = ir.done();
    if (countIR > 0) {
      
        countResend = countIR;
        const byte* data = ir.buffer();
        switch (ir.decoder(countIR)) {
  
            default:
                for (byte i = 0; i < countIR; ++i) {
                    byte nibble = (data[i/2] >> (i % 2) * 4) & 0x0F;
                    Serial.print(nibble, HEX);
                }
                Serial.println("");
                resendIR();
        }
       
    }
    ir.done();
    //
  
    
    //--------------------------------------RF12 scanning
    char* buf = (char*) rf12_data;
  if (rf12_recvDone() && rf12_crc == 0) {
    unsigned char n = 0;
    unsigned char b = 0;
    unsigned char m = 0;
    unsigned char r = 0;
    unsigned char p = 0;
    unsigned char c = 0;
    String name = "          ";
    String binary = "  ";
    String mass = "     ";
    String radius = "     ";
    String period = "     ";
    for (int i = 0; i<30; i++){
      if ( buf[i]=='*') c++;
      if ( c==1 ) name[n++] = buf[i];
      if ( c==2 ) binary[b++] = buf[i];
      if ( c==3 ) mass[m++] = buf[i];
      if ( c==4 ) radius[r++] = buf[i];
      if ( c==5 ) period[p++] = buf[i];
    }
    
if ( checkMass(mass) ) {
  Serial.println("*-------------------ok");//testing: found nce exoplanet!!!!
  Serial.println('d', HEX);//signal to middle LED's
  
}

    Serial.println( name );
    Serial.println( binary );
    Serial.println( mass );
    Serial.println( radius );
    Serial.println( period );
  }
    
}

boolean checkMass(String mass){
   for (int i = 1; i<10; i++){
      if ( (mass[i]=='.') && ( mass[i-1]=='0'))
  return true;
   }
   return false;
}

 

void sendingText(unsigned char* myBuf ){
  
  unsigned char buf[] = "                              " ;
  unsigned char* bufPtr;
  bufPtr = myBuf;
  unsigned char i = 1;
  buf[0] = *bufPtr;
  while ( *(bufPtr++) = *(myBuf++) ) buf[i++] = *bufPtr ;
  
  while (!rf12_canSend()) rf12_recvDone(); rf12_sendStart(0, buf, sizeof buf);
}

void printTime(){
  
   byte now[6];
    getDate(now);
    
      for (byte i = 0; i < 6; ++i) {
        Serial.print(' ');
        Serial.print((int) now[i] );
        delay(10);
    }
    
    unsigned char buf[] = "wait a sec    " ;
    buf[12] = ((int)now[5])/10 + 48;
    buf[13] = ((int)now[5])%10 + 48;
    sendingText( buf );
    
 if ( (now[6] == now[5]) && (now[4] == now[5] ))
   doTimeParty( now[5]);
  
}

void doTimeParty(byte nowEqualTime){
  
  //printing teh same 6 times
   for (byte i = 0; i < 6; ++i) {
        Serial.print(' ');
        Serial.print((int) nowEqualTime );
        delay(10);
    }
    Serial.println();
    
    for (byte i = 0; i < 0x0F; ++i) {
        Serial.print(i);
        delay(10);
    }
    Serial.println();
}

 static byte bin2bcd (byte val) {
    return val + 6 * (val / 10);
}

static byte bcd2bin (byte val) {
    return val - 6 * (val >> 4);
}

static void setDate (byte yy, byte mm, byte dd, byte h, byte m, byte s) {
    rtc.send();
    rtc.write(0);
    rtc.write(bin2bcd(s));
    rtc.write(bin2bcd(m));
    rtc.write(bin2bcd(h));
    rtc.write(bin2bcd(0));
    rtc.write(bin2bcd(dd));
    rtc.write(bin2bcd(mm));
    rtc.write(bin2bcd(yy));
    rtc.write(0);
    rtc.stop();
}
 static void getDate (byte* buf) {
	rtc.send();
	rtc.write(0);	
    rtc.stop();

	rtc.receive();
    buf[5] = bcd2bin(rtc.read(0));
    buf[4] = bcd2bin(rtc.read(0));
    buf[3] = bcd2bin(rtc.read(0));
    rtc.read(0);
    buf[2] = bcd2bin(rtc.read(0));
    buf[1] = bcd2bin(rtc.read(0));
    buf[0] = bcd2bin(rtc.read(1));
    rtc.stop();
}


