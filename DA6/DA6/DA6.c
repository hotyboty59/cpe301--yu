/*
 * DA6.c
 *
 * Created: 4/16/2013 3:38:25 PM
 *  Author: Ting Yu
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>               
#include <avr/io.h>
int main()
{
	//PORTD pins as input
	DDRD = 0x00;
	//Enable internal pull ups
	//PORTD = 0xFF;
	//Set PORTB1 pin as output
	DDRB = 0xFF;
	
	DDRC    =   0x00;                   // C ports are set
	ADCSRA  =   0x87;
	ADMUX   =   0x40;
	
	//TOP = ICR1;
	//output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM
	//ICR1 = 20000 defines 50Hz pwm
	ICR1 = 20000;
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);    //TCCR1A = 0x82
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);                        //TCCR1B = 0x1A
	//start timer with prescaler 8
	
	while(1)                                        //begins loop
	{
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA& (1<<ADIF)) == 0);         // begins loop
		
		float speed =   ADC;
		speed *= 1.8;                               // speed is calculated
		speed+= 530;

 		
		OCR1A = speed;
		_delay_ms(10);                 //delay set to 10ms
		
		
	}
}