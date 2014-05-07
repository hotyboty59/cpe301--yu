/*
 * DA8.c
 *
 * Created: 5/5/2014 1:50:27 PM
 *  Author: Ting Yu
 */ 


#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

volatile uint16_t outputV = 0;	
volatile uint8_t phase = 0;	
volatile uint8_t delta = 20;	
volatile uint8_t z = 0;	
volatile uint8_t v = 0;	


//This is a table to look up the values for the inverse of the sine for Pi/2 degrees.
static const uint8_t sineTable[] PROGMEM =
{
	40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,
	41,41,41,41,41,41,41,41,42,42,42,42,42,42,42,43,43,43,43,43,44,44,
	44,44,45,45,45,46,46,46,46,47,47,48,48,48,49,49,50,50,51,51,52,52,
	53,54,54,55,56,57,58,58,59,61,62,63,64,65,67,69,70,72,74,77,79,82,
	85,89,94,99,105,112,122,135,152,180,235
};

//waits for the TWI to finish
uint8_t i2c_wait(){
	while(!(TWCR & (1<<TWINT)));
	return (TWSR&0xF8);
}


//sends data 
uint8_t i2c_send(uint8_t data){
	TWDR = data; //Set the data.
	TWCR = (1<<TWINT) | (1<<TWEN); //Enable TWI.
	return i2c_wait(); //Wait for TWI to finish.
}

//ends the transfer
void i2c_end(){
	if(i2c_send(0xF0)!=0x28) return;
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

//sets values 
void i2c_set(uint8_t m, uint8_t l){
	if(i2c_send(m&0xF)!=0x28){
		return;
	}
	if(i2c_send(l)!=0x28){
		return;
	}
}


int main(void){
	ADCSRA = 0xC0;	//Starts ADC.
	ADMUX = 0x40;
	TCCR1A = 0x00;	 //Starts timer
	TCCR1B = 0x09;
	TIMSK1 = 0x02;
	OCR1A = 250;
	sei();
	
	TWBR = 0;	//sets the max speed
	TWSR = 0; //Initialize TWI function
	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); //start trans for the MCP4725 function
	if(i2c_wait()!=0x08) return;
	if(i2c_send(0xC0)!=0x18) return;
	
	while(1){	//Continuously output to MCP
		cli();	//read 
		uint16_t num = outputV;
		sei();
		i2c_set((uint8_t)((num>>8)&0x0F),(uint8_t)(num&0xFF));	//write to the MCP4725
	}
}


ISR(TIMER1_COMPA_vect){
	cli();
	if(!(ADCSRA&(1<<ADSC))){	//checks if the ADC has finished
		delta = 8+(ADC>>4);	//sets the number of ticks to increment
		ADCSRA = 0xC0;
		ADMUX = 0x40;
	}

	uint8_t x = pgm_read_word(&sineTable[(phase&1)?98-v:v]);	//read from the inverse sine and adjust it with the phase
	if(z+delta < x && z < x-delta){
		z += delta;
	}
	else{
		z += delta;
		//keep incrementing the states
		do{
			z -= x;
			v++;
			if(v >= 99){	//update phase
				v = 0;
				phase = (phase+1)&3;
			}
			x = pgm_read_word(&sineTable[(phase&1)?98-v:v]);	//inverse the sine
		}
		
		while(z > x);
		uint16_t temp = v;
		switch(phase){	//transforms the output based on scale and phase
			case 0: temp += 99; break;
			case 1: temp = 198-temp; break;
			case 2: temp = 99-temp; break;
		}
		temp <<= 4;
		outputV = temp;
	}
	sei();
}
