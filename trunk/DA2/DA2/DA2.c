/*
 * DA2.c
 *
 * Created: 2/28/2014 1:48:20 PM
 *  Author: Ting Yu
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
void delay(void);

int main(void)
{
	SP = RAMEND; //Initialize the stack
	DDRB = 0xFF; //Port B as output for the counter
	DDRC = 0xFF; //Port C as output for every 9th bit
	DDRD = 0x00; //Port D as input 
	TCNT0 = 0x00;
	EIMSK = 0x01; //enable int0 as interrupt
	EICRA = 0x02; //trigger on falling edge
	sei(); //enable interrupt
		
    while(TCNT0 < 256)
    {
		delay();
		//PORTC = 0x00;
		TCNT0 ++;   //increment count
		PORTB = TCNT0;   //output the count to Port B
		if(TCNT0 %10 == 0){ //if for 10th time
		PORTC ^= (1<<0);  //Toggle PC0
		PORTC ^= 100; //PC2 = PC 0
		}
    }
	return 0;
}

void delay(void){ //_delay_ms(2500);
	//this function will use timer1 to make 0.25 sec delay
	//TCNT = 2^16 - (0.25 at 8MHz)/Prescalar
	//TCNT = 2^16 - 2*10^6 / 64 = 57723 = E17B
	TCCR1A = 0x00;
	TCCR1B = 0x03; //normal mode Prescalar of 64
	TCNT1 = 0xE17B;
	while ((TIFR1&0x01)== 0x00)
	{
		//wait
	}
	TIFR1 = 0x01;  //reset the counter
	return;
}


ISR (INT0_vect){
	//This will be called every falling edge of INT0
	TCNT2++;
	if(TCNT2 == 0x02){
		PORTC ^= (1 << 1); //Toggle PC1
		TCNT2 = 0x00;
	} 
}