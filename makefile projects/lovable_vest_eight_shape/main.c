#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU  8000000L 

#include <util/delay.h>

/* Prototypes */

void InitUART (unsigned char baudrate);
unsigned char ReceiveByte (void);
void TransmitByte (unsigned char data);
void fancy();


#define TIMER1_PRESCALE_64 2
 

#define HIGH 0
#define LOW 1

#define HEX__(n) 0x##n##UL
#define B8__(x) ((x&0x0000000FLU)?1:0)  \
               +((x&0x000000F0LU)?2:0)  \
               +((x&0x00000F00LU)?4:0)  \
               +((x&0x0000F000LU)?8:0)  \
               +((x&0x000F0000LU)?16:0) \
               +((x&0x00F00000LU)?32:0) \
               +((x&0x0F000000LU)?64:0) \
               +((x&0xF0000000LU)?128:0)
#define B8(d) ((unsigned char)B8__(HEX__(d)))

unsigned char col = 0;
unsigned char row = 0;
volatile unsigned char counter = 0;
volatile unsigned char counterPrescaler = 0;
volatile unsigned char counterP = 2;

unsigned char  cols[5] = { 4, 3 , 2, 1, 0 };
unsigned char  rows[5] = {9, 8, 7,  6, 5 };

unsigned char leds[5] ;

int counterSign = 1;

void delay_ms( uint16_t milliseconds);
void clearLeds() ;
void setPattern();
void digitalWrite(unsigned char pinNum, unsigned char highLow) ;

void digitalWrite(unsigned char pinNum, unsigned char highLow) {
	if ( highLow ==  0 )
	{
		 switch ( pinNum ) {
			case 9: PORTD &= ~_BV(PD5); break;
			case 8: PORTD &= ~_BV(PD6); break;
			case 7: PORTB &= ~_BV(PB0); break;
			case 6: PORTB &= ~_BV(PB1); break;
			case 5: PORTB &= ~_BV(PB2); break;
			case 4: PORTB &= ~_BV(PB3); break;
			case 3: PORTB &= ~_BV(PB4); break;
			case 2: PORTB &= ~_BV(PB5); break;
			case 1: PORTB &= ~_BV(PB6); break;
			case 0: PORTB &= ~_BV(PB7); break;
 
		 }
	 }
	 else
	 {
		 switch ( pinNum ) {
			case 9: PORTD |= (1<<PD5); break;
			case 8: PORTD |= (1<<PD6); break;
			case 7: PORTB |= (1<<PB0); break;
			case 6: PORTB |= (1<<PB1); break;
			case 5: PORTB |= (1<<PB2); break;
			case 4: PORTB |= (1<<PB3); break;
			case 3: PORTB |= (1<<PB4); break;
			case 2: PORTB |= (1<<PB5); break;
			case 1: PORTB |= (1<<PB6); break;
			case 0: PORTB |= (1<<PB7); break;

		 }
	 }
}

void setPattern() {


 leds[4] = 0xff;
 leds[3] = 0xff;
  leds[2] = 0xff;
   leds[1] = 0xff;
   //unsigned char cMIN = counter - 1;
   
if ( counter >=  0  && counter < 5 ) leds[4] = 0xff - (1 << ((counter%5)    ));//- (1 << ((cMIN%5)-1)) ;
if ( counter >=  5  && counter < 10 ) leds[3] = 0xff - (1 << (4 - ((counter%5) )));//- (1 << (5 - ((cMIN%6) ))) ;
if ( counter >=  10  && counter < 15 ) leds[1] = 0xff - (1 << ((counter%5)   ));// - (1 << ((cMIN%6)-1)) ;
if ( counter >=  15  && counter < 20 ) leds[2] = 0xff - (1 << (4 - ((counter%5) )));//- (1 << (5 - ((cMIN%6) )));

if (counter == 19 ) counter = 0; else counter += counterSign;

//if (counter == 0 ) counterSign = - counterSign;
}


void clearLeds() {
  for (unsigned char i = 0; i < 5; i++)
      leds[i] = 0x00;
}


void delay_ms( uint16_t milliseconds)
{
   for( ; milliseconds > 0; milliseconds--)
   {
      _delay_ms( 1);
   }
}

int main (void)
{

	DDRB = 0xFF;			// Set output.
	DDRD = 0xFF;
	DDRD|= (1<<PD5);
	DDRD|= (1<<PD6);

	PORTD = 0x00;
	PORTB = 0x00;

	clearLeds();

  TCCR0B = (1 << WGM02) | counterP; //WGNM12 bit3 of tccr1b timer control register
  OCR0A = (uint16_t)2000;//output compare register comined with WGM02 set
  TIMSK |= 1 << OCIE0A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
  
  	sei();
  	InitUART (51);  //for communicating with arduino at 16000 attiny2313 on 8000

	fancy();

	 while (1)
    {

	unsigned char inChar = ReceiveByte ();
	counterPrescaler++;
	setPattern();

	delay_ms(30);

	if ( inChar =='f' | inChar =='F' )
		{
		 	fancy();
		}

	 if ( counterPrescaler%50 == 0  )
	 	{
			cli();
			TCCR0B = (1 << WGM02) | counterP; //WGNM12 bit3 of tccr1b timer control register
			OCR0A = (uint16_t)2000;//output compare register comined with WGM02 set
			TIMSK |= 1 << OCIE0A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
			counterP++;
  			if ( counterP > 4 ) counterP = 2;
			sei();
			delay_ms(200);
	 }
 
	}
}

void fancy(){
	for ( unsigned char c = 0; c<25; c++){
			setPattern();
			counterPrescaler++;
			delay_ms(50);
			}
}

// Interrupt routine
SIGNAL( TIMER0_COMPA_vect   )  {

		// extendedDigitalWrite( cols[col], HIGH );  // Turn whole previous column off
	digitalWrite( rows[row], LOW );

  	row++;
  	if (row == 5)
    row = 0;

  	for (unsigned char col = 0; col < 5; col++) {

	    if (  (leds[row]  & (1 << col )) == HIGH )
	     	digitalWrite( cols[col], LOW);  // Turn on this led
	    else
	     	digitalWrite( cols[col], HIGH); // Turn off this led
	  	}
 
	digitalWrite( rows[row],  HIGH);
}

/* Initialize UART */
void InitUART (unsigned char baudrate)
{
  /* Set the baud rate */
  UBRRL = baudrate;

  /* Enable UART receiver and transmitter */
  UCSRB = (1 << RXEN) | (1 << TXEN);

  /* set to 8 data bits, 1 stop bit */
  UCSRC = (1 << UCSZ1) | (1 << UCSZ0);

}

/* Read and write functions */
unsigned char ReceiveByte (void)
{

  /* Wait for incomming data */
  while (!(UCSRA & (1 << RXC))   );

  /* Return the data */

  return UDR;

}

void TransmitByte (unsigned char data)
{
  /* Wait for empty transmit buffer */
  while (!(UCSRA & (1 << UDRE)));
//	blinkEm(100,0);
  /* Start transmittion */
  UDR = data;
}

