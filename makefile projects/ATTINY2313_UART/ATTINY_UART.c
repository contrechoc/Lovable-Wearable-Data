#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU  8000000L 

#include <util/delay.h>

/**********************************************************
*
* testing UART transmission between two ATTINY2313
* external crystals
* not all F_CPU work well!
* connection 
*
*********************************************************/

/* Prototypes */

void InitUART (unsigned char baudrate);
unsigned char ReceiveByte (void);
void TransmitByte (unsigned char data);
void sendCodeToSLaves(unsigned char indicator, unsigned char offSetForIndicator, unsigned char signCounter);


int clig = 0;
unsigned char counter = 0;

int main (void)
{
   	//unsigned char data[] = "hello world nice";

	counter = 0;

  	DDRB = 0xFF;			// Set PORTB to all output.
	DDRD = 0xFF;
	PORTD |= (1<<PD6);

 InitUART (51);     /* Set the baudrate to 2400 bps using a 2000MHz crystal */

  while (1)
    {


//tested with 150 receiving and sending 50 works
//tested with 150 receiving and sending 25 works
//tested with 150 receiving and sending 5 works skips a lot
//tested with 25 receiving and sending 5 works ?
//tested with 25 receiving and sending 15 works
 



	if (1== 0) //either transmitting or receiving
	{

	PORTB = ReceiveByte ();

	_delay_ms(2);

	if ( counter++ == 17 ) {
			counter = 0;
			if ( clig == 0){
			 	PORTD &= ~_BV(PD6);
				clig = 1;
			 }
			 else{
				PORTD |= (1<<PD6);
				clig = 0;
			}
		_delay_ms(1000);
		}


	}
	else
	{

sendCodeToSLaves(100, 38, counter);
sendCodeToSLaves(101, 39, counter);
sendCodeToSLaves(102, 40, counter);
sendCodeToSLaves(103, 41, counter);
sendCodeToSLaves(104, 42, counter);

	counter++;
	if ( counter > 36 ) {
			
			if ( clig == 0){
					PORTD &= ~_BV(PD6);
					clig = 1;
 			}
 			else{
					PORTD |= (1<<PD6);
					clig = 0;
			}
			_delay_ms(500);
			counter = 0;
			}

	_delay_ms(150); // DOES NOT work with lower delay, checked with serial port of arduino
 


	}

    }

}

void sendCodeToSLaves(unsigned char indicator, unsigned char offSetForIndicator, unsigned char signCounter){
	//indicator = 100;
	TransmitByte ( indicator );
	_delay_ms(50);

	//offSetForIndicator = 43;
	TransmitByte ( offSetForIndicator );
	_delay_ms(50);

	TransmitByte ( signCounter );

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
  while (!(UCSRA & (1 << RXC)));

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
