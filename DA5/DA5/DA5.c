/*
 * DA5.c
 *
 * Created: 4/4/2014 12:38:05 AM
 *  Author: Ting Yu
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>
#define MOSI 3												//defines the output pin for MOSI
#define SCK 5												//defines the output pin for SCK
#define SS 2												//defines the output pin for SS

void usart_init (void)
{
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);							//enable receiver and transmitter
	UCSR0C = ((1<<UCSZ01)|(1<<UCSZ00)|(0<<UMSEL00));
	UBRR0L = 0x33;
}

void usart_send (unsigned char ch)
{
	while(!(UCSR0A & (1<<UDRE0)));							//waiting for buffer to be clear
	UDR0 = ch;
}

void execute (unsigned char cmd, unsigned char data)
{
	PORTB &= ~(1<<SS);											//initializes the packet by pulling SS low
	SPDR = cmd;													//begins CMD transmission
	while(!(SPSR & (1<<SPIF)));									//just waiting for the transfer to finish
	SPDR = data;												//begins DATA transmission
	while(!(SPSR & (1<<SPIF)));									//waiting for the transfer to finish
	PORTB |= (1<<SS);											//terminates the packet by pulling SS high
}

int main(void)
{
	ADCSRA |= (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);   //enable ADC and select clk/128
	ADMUX |= (1<<REFS0)|(1<<ADLAR);							//Vref, ADC0 as input
	DDRD = 0xFF;											//Enable Port D for output
	
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);						//MOSI and SCK set as output
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);					//SPI set as master
    
	execute(0x09,0x0F);			// enable decoding for digits 1, 2, 3, and 4
	execute(0x0B,0x04);			// scan two 7-segments
	execute(0x0C,0x01);			// turn on the chip
	usart_init();
	int temp;
	
	while(1)
    {
		ADCSRA |= (1<<ADSC);				//begin conversion
		while (!(ADCSRA |= (1<<ADSC))==0);
		_delay_ms(1000);
		//1 degree = 10mV, 0.1V = 0x05 in ADCH
		// 12 in ADCH = 0.24V = 24C
		temp = ADCH*2;								//calculation for the temp
		execute(0x04, temp/1000);					//1st digit
		execute(0x03, temp%1000/100);				//2nd digit
		execute(0x02, temp%100/10 | (1<<7));		//3rd digit
		execute(0x01, temp%10);						//4th digit
		
		//usart_send to monitor
		usart_send(temp/10);						// send temp
		usart_send(70);								// used for 'F' in Fahrenheit
		usart_send(32);								// used for blank space
		usart_send(32);								// used for blank space
		_delay_ms(1000);							// Display temperature		
	
    }
	return 0;
}