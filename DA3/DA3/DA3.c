/*
 * DA3.c
 *
 * Created: 3/11/2014 1:37:21 PM
 *  Author: Ting Yu
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t count = 0;

int main(void)
{

	DDRD = 0b11000000;  //make PD6 and PD7 as output
	DDRB = 0b11111111;  //Port B as output for the counter
	TCCR1A = 0b01000000;
	TCCR1B = 0b00001011;   //CTC mode prescaler 64
	TIMSK1 = 0b00000010;   //timer1 interrupt mask reg
	OCR1A = 7812;

	TCCR0A = 0b00000000;
	TCCR0B = 0b00000111;  //set external clock source
	EIMSK = 0x01; //enable int0 as interrupt
	EICRA = 0x02; //trigger on falling edge
	sei(); //enable interrupt
	
	while(1)
	{
		//wait for interrupt
		PORTB = TCNT0;   //output the count to Port B
	}
}

ISR (INT0_vect)
{
	//This will call every falling edge of INT0
	count++;
	if (count == 16)
	{
		count = 0;
		PORTD ^= 1<<7;  //output to 9th bit LED
	}
}


ISR (TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	PORTD ^= 1<<6;  //Toggle PD6, connect to PD4 for timer0 clock input
}