/*
 * DA6.c
 *
 * Created: 4/16/2013 11:17:38 AM
 *  Author: Ting Yu
 */ 

#define F_CPU 8000000UL                             //XTAL = 8 MHZ
#include <avr/io.h>                                   //standard AVR header
#include <util/delay.h>                               //delay header
void delay_ms(int count);


int main()
{
	
	DDRB    =   0xFF;                       // B ports set
	DDRD    =   0xFF;                       // D ports set
	DDRC    =   0x00;                       // C ports set
	ADCSRA  =   0x87;
	ADMUX   =   0xC0;
	
	while(1)
	{
		
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA& (1<<ADIF)) == 0);     //loop begins
		
		int speed   =   ADC;                    // declared int = ADC
		speed+= 10;
		
		PORTB   = 0x11;                         //port B set for speed of motor
		delay_ms(speed);                  //delay speed 
		PORTB   = 0x22;                         //port B set for speed of motor
		delay_ms(speed);                  //delay speed 
		PORTB   = 0x44;                         //port B set for speed of motor
		delay_ms(speed);                  //delay speed 
		PORTB   = 0x88;                         //Port B set for speed of motor
		delay_ms(speed);                  //delay speed 
		
		
	}
}
void delay_ms(int count)
{
	for (int i=0; i < count; i++)
	_delay_ms(1);
}
