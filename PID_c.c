/*
 * PID_C.c
 *
 * Author : AK
 */ 



//defines
#define F_CPU 16000000UL
#define BAUD 115200
#define UBBRF F_CPU/8/BAUD-1

//includes
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>

 

//ADC GLOBAL VARIABLES//
double sampleRate = 30000;
double Vsample;
unsigned int ADC_FLAG = 0;
unsigned int i = 0;



//PID GLOBAL VARIABLES//
double integralErr,diffErr,prevErr;
double Vref = 76.5,Verr;
double kp = 50,kd = 5,ki = 45;
double output;


void initADCTimer (){
	
	TCCR1B |= (1<<CS11)| (1<<CS10);								// 250kHz ADC frequency
	TCCR1B |= (1<<WGM12);									// Wavegeneration CTC mode 4.
	TIMSK1 |= (1<<OCIE1B);									// timer counter 1 Output compare interrupt enable.
	OCR1B  = ((F_CPU/(64*sampleRate))-1);
	OCR1A  = ((F_CPU/(64*sampleRate))-1);

	ADCSRA |= (1<<ADSC); // start AD conversion.
	
}

void initADC (){
	ADCSRA |= (1<<ADEN) /*| (1<<ADPS2)*/ |(1<<ADIE) | (1<<ADATE);				// ADC ENABLE
	ADCSRA |= ((1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2));						// Prescale by 128
	ADCSRB |= (1<<ADTS2) | (1<<ADTS0);							// Trigger source: Timer counter/compare match interrupt
	ADMUX  |= (0<<REFS1)| (1<<REFS0);							// REFS1 = 0 og REFS0 = 1 for 5V reference
	ADMUX  |= (1<< ADLAR);									// Left adjusted data register
	
	DIDR0 =~ DIDR0;
	
	DIDR2 = 0xff;										// Set pins ADC8..15 disabled
}

void initPWMTimer(double dutyCycle){

	DDRB |= (1<<PB7);									// DDRB PB7 set as output
	TCCR0A |= (1<<COM0A1)|(1<<WGM00)|(1<<WGM01);						// waveform generation mode 2 fast-PWM
	TCCR0B |= (1<<CS00);									// prescale clock by 0. 62500 Hz
	TIMSK0 |= (1<<TOIE0);									// Timer Overflow Interrupt Enable
	OCR0A = dutyCycle;									// Set output compare as the dutycycle

}

//////////////////// Uncomment for debugging interface through UART //////////////////////////
/*
void UART_Init(unsigned int ubrr)
{
	UCSR0A = (1<<U2X0);									// Full duplex
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);							// Enable Receive and transmit
	UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);							// Frame: 1 start bit, 8 data bit, no parity: one stop bit
	UBRR0H = (unsigned char) (ubrr>>8);							// Baud rate values up to 16 bit therefore to register
	UBRR0L = (unsigned char)ubrr;
}

void putchUSART0 (char tx)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = tx;
}


void putsUSART0(char *ptr){
	while(*ptr)
	{
		putchUSART0(*ptr);
		ptr++;
	}
}
*/


int main(void)
{
	sei();											// Global interrupt enable
	/*UART_Init(UBBRF);*/									// Uncomment for debugging use
	initPWMTimer(128);									// PWM timer initialization
	initADC();										// ADC initialization
	initADCTimer();										// ADC Timer initialization and ADC start
	
   
   
    while (1) 
    {
		
		Verr = Vref - Vsample;								// Error between set voltage reference and measured voltage
												// for the proportional term

		integralErr = (integralErr+Verr)/sampleRate;					// calculation of integral term
		
		if(integralErr>5000){								// limitation of integral term [-5000 , 5000]
			integralErr= 5000;
		}
		if(integralErr<-5000){
			integralErr=-5000;
		}

		diffErr = (Verr-prevErr)*sampleRate;						// calculation of differential term
		
		output =( kp*Verr)+(ki*integralErr)+(kd*diffErr);				// calculation of output
		if(output>255){									// limit output [0 , 255]
			output = 255;
		}
		
		if(output<0){
			output = 0;
		}
		
		prevErr = Verr;									// current error is save for next PID call.
		OCR0A = 255-output;
		/*initPWMTimer(output);*/
		
    }
}

ISR(ADC_vect){
	Vsample = ADCH;										// only MSB from ADCH is read
	
	//ADC_FLAG = 1;
}


ISR(TIMER0_OVF_vect){										// ISR for Timer0 overflow empty for efficiency
	
}

ISR(TIMER1_COMPB_vect){										// ISR for Timer1 compare match B also empty for efficiency 
	
}
