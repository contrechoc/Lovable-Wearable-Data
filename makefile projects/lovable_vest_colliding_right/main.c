#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU  8000000L 

#include <util/delay.h>

#define TIMER1_PRESCALE_64 2
 

#define HIGH 0
#define LOW 1
 
unsigned char col = 0;
unsigned char counter = 0;
unsigned char counterPrescaler = 0;
unsigned char counterP = 2;

unsigned char bitCounter = 0;

unsigned char  cols[5] = { 4, 3, 2, 1, 0 };
unsigned char  rows[5] = { 9, 8, 7, 6, 5 };

unsigned char leds[5] ;

void delay_ms( uint16_t milliseconds);
void clearLeds() ;
void setPattern();
void digitalWrite(unsigned char pinNum, unsigned char highLow) ;
void InitUART (unsigned char baudrate);
unsigned char ReceiveByte (void);
void TransmitByte (unsigned char data);
void fancy();


void digitalWrite(unsigned char pinNum, unsigned char highLow) {
	if ( highLow == 0 )
	{
		 switch ( pinNum ) {
			case 0: PORTD &= ~_BV(PD5); break;
			case 1: PORTD &= ~_BV(PD6); break;
			case 2: PORTB &= ~_BV(PB0); break;
			case 3: PORTB &= ~_BV(PB1); break;
			case 4: PORTB &= ~_BV(PB2); break;
			case 5: PORTB &= ~_BV(PB3); break;
			case 6: PORTB &= ~_BV(PB4); break;
			case 7: PORTB &= ~_BV(PB5); break;
			case 8: PORTB &= ~_BV(PB6); break;
			case 9: PORTB &= ~_BV(PB7); break;
 
		 }
	 }
	 else
	 {
		 switch ( pinNum ) {
			case 0: PORTD |= (1<<PD5); break;
			case 1: PORTD |= (1<<PD6); break;
			case 2: PORTB |= (1<<PB0); break;
			case 3: PORTB |= (1<<PB1); break;
			case 4: PORTB |= (1<<PB2); break;
			case 5: PORTB |= (1<<PB3); break;
			case 6: PORTB |= (1<<PB4); break;
			case 7: PORTB |= (1<<PB5); break;
			case 8: PORTB |= (1<<PB6); break;
			case 9: PORTB |= (1<<PB7); break;

		 }
	 }
}

void setPattern() {

	if ( bitCounter > 5 ) 
	{
		bitCounter = 0;
		counter ++;
	}

	  if ( counter%3 == 0 ) leds[0] = ~( 0B00000001 << (bitCounter++) ) ;//row 1, green row off when 0xff
	  if ( counter%3 == 1 ) leds[1] = ~( 0B00000001 << (bitCounter++) ) ;
	  if ( counter%3 == 2 ) leds[2] = ~( 0B00000001 << (bitCounter++) ) ;

}


void clearLeds() {

unsigned char i = 0;
  for ( i = 0; i < 5; i++)
      leds[i] = 0xff;
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

  TCCR0B = (1 << WGM02) | TIMER1_PRESCALE_64; //WGNM12 bit3 of tccr1b timer control register
  OCR0A = (uint16_t)200;//output compare register comined with WGM02 set
  TIMSK |= 1 << OCIE0A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
  
  sei();
  InitUART (51);  //for communicating with arduino at 16000 attiny2313 on 8000

	fancy();

	 while (1)
    {

 		unsigned char inChar = ReceiveByte ();
      		counterPrescaler++;

		setPattern();

	 	if ( inChar =='C' || inChar =='c' )
			{
		 	fancy();
			}

 

 
	  if ( counterPrescaler%35 == 0)
	//	if ( 1 == 0 )
	 	{
			cli();
			TCCR0B = (1 << WGM02) | counterP; //WGNM12 bit3 of tccr1b timer control register
			OCR0A = (uint16_t)200;//output compare register comined with WGM02 set
			TIMSK |= 1 << OCIE0A;   // Output Compare Interrupt Enable (timer 1, OCR1A)
			counterP++;
			delay_ms(200);
  			if ( counterP > 4 ) counterP = 2;	
			sei();
	 	}
	 
 
	}
}

void fancy(){
	unsigned char c = 0;
	for ( c = 0; c<25; c++){
			setPattern();
			counterPrescaler++;
			delay_ms(50);
			}
}

// Interrupt routine
SIGNAL( TIMER0_COMPA_vect   )  {

	digitalWrite( cols[col], LOW );
  	col++;
  	if (col == 5)
    col = 0;

unsigned char row = 0;
  	for ( row = 0; row < 5; row++) {

	    if (  (leds[col] & (1 << row )) == HIGH )
	     	digitalWrite( rows[row], LOW);  // Turn on this led
	    else
	     	digitalWrite( rows[row], HIGH); // Turn off this led
	  	}

	digitalWrite( cols[col],  HIGH);
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


