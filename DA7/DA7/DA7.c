/*
 * DA7.c
 *
 * Created: 4/22/2014 11:31:09 PM
 *  Author: tingyu
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "nokia5110.h"
#include "main.h"


int main()
{

        LcdInit();	//initialize  LCD
        LcdContrast(0x7F);	 //display image
        _delay_ms(1000);
        LcdClear();
       
        LcdContrast(0x3F);	//adjust contrast
        _delay_ms(1000);
        LcdClear();
        
        LcdImage(waitImage);	//wait image
        LcdUpdate();
        _delay_ms(4000);
		
		ADCSRA |= (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);   //enable ADC and select clk/128
		ADMUX |= (1<<REFS0)|(1<<ADLAR);							//Vref, ADC0 as input
		DDRC = 0x00;
		
		uint16_t temp, temp1, temp2, temp3;

        while (1)
        {
				ADCSRA |= (1<<ADSC);	//begin conversion
				while(!(ADCSRA &(1<<ADSC))==0);		//wait for end of conversion
				temp=(ADCH<<8)|ADCL;
				//calculation for temperature
                temp1 = temp%10;
                temp2 = temp%100/10;
                temp3 = temp%1000/100;
                char digit[4];
                sprintf(digit, "%d%d.%d ", temp1, temp2, temp3); //display temperature
                LcdClear();
                LcdGotoXYFont(1,1);
                LcdFStr(FONT_1X,(unsigned char*)digit);
                LcdGotoXYFont(1,2);
                LcdFStr(FONT_1X,(unsigned char*)PSTR ("Fahrenheit"));
                LcdUpdate();
                _delay_ms(1000);
        }
        return 0;
}