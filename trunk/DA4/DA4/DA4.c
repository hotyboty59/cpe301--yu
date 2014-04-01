/*
 * DA4.c
 *
 * Created: 3/29/2014 10:13:12 PM
 *  Author: Tingyu
 */ 


#define F_CPU 1000000UL										//XTAL = 1 MHz
#include <util/delay.h>										//util, delay library
#include <avr/io.h>											// library for avr/io
int main ()
{
	while (1)												//wait here forever after dimming is done
	{
		unsigned char R, G, B;								// var declarations
		DDRB = 0xFF;									//PB3 as output
		R = 255;										// R set to 255
		B=0;											//B set to 0
		OCR1A = 255;									//duty cycle = 50%
		TCCR1A = 0xC1;									//Phase Correct PWM, inverted, N = 64
		TCCR1B = 0x03;
		OCR2A = 255;									//duty cycle = 50%
		TCCR2A = 0xC1;									//Phase Correct PWM, inverted, N = 64
		TCCR2B = 0x03;
		OCR0A = 127;									//duty cycle = 50%
		TCCR0A = 0xC1;
		TCCR0B = 0x03;

		while (R != 0 )									// starst loop2
		{
			OCR1A = R;
			_delay_ms(10);									//use AVR studio library delay
			while (B !=255)
			{
				OCR2A = B;
				_delay_ms(10);
				B++;								// B is incremented
			}
			R--;									// R is decremented
		}

		DDRB = 0x02;								    //PB3 as output
		DDRD = 0xff;
		R = 255;										// R set to 255
		G=0;											// G is set to 0
		OCR1A = 255;									//duty cycle = 50%
		OCR2A = 127;									//duty cycle = 50%
		OCR0A = 255;									//duty cycle = 50%

		while (R != 0 )									// starts loop 3
		{
			OCR1A = R;
			_delay_ms(10);								    //use AVR studio library delay
			while (G !=255)									//start of 4th loop
			{
				OCR0A = B;
				_delay_ms(10);								//delay of 25ms
				G++;										//G is incremented
			}
			R--;											// R is decremented
		}

		DDRB = 0x04;									//PB3 as output
		DDRD = 0xff;
		R = 255;
		G=0;
		OCR1A = 127;									//duty cycle = 50%
		OCR2A = 255;									//duty cycle = 50%
		OCR0A = 255;									//duty cycle = 50%
		while (R != 0 )								// stars loop 5th
		{
			OCR0A = R;
			_delay_ms(10);									//use AVR studio library delay
			while (G !=255)									// starts 6th loop
			{
				OCR2A = B;									// sets register to B
				_delay_ms(10);								// delay of 100ms
				G++;										// G is incremented
			}
			R--;												// R is decremented
		}
	}
	return 0;
}