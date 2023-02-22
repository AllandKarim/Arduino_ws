/*
 * timer.c
 *
 * Created: 18-10-2021 01:18:03
 * Author : AllandKarim 
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TimerCompareMatch ((F_CPU/(64*SampleRate))-1) 


unsigned int SampleRate = 1000;                         	   // 1000 hz samplerate (1000 samples per second i.e. 1 millisecond between each sample.)
unsigned int ms = 0;
unsigned int s = 0;
unsigned int min = 0;
unsigned int h = 0;


void init_timer1();

int main(void)
{
	sei();

	init_timer1();


    while (1) 
    {

	if(ms==999){                                   	    	    // if 1000 milliseconds
		s++;                                       	    // increment second
		ms = 0;                                     	    // reset millisecond counter

	}

	if(s==59){                                          	    // if 60 seconds
		min++;                                     	    // increment minute
		s = 0;                                      	    // reset seconds counter
	}

	if (min == 59){                                     	    // if 60 minutes
		h++;                                        	    // increment hour
		min = 0;                                    	    // reset minutes
	}

	if(h == 23 && min == 59 && s == 59){              	    // if 24 hours
		h = 0;                                     	    // reset hours

	}
	
    }
}

ISR(TIMER1_COMPA_vect){
	
	ms++;                                               	    // increment millisecond every interrupt
								   
}								   
								   
void init_timer1(){						   
	TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);      	    // wave generation mode with prescaler 64
	OCR1A = TimerCompareMatch;                          	    // set output compare register
	TIMSK1 |= (1 << OCIE1A);                            	    // enable output compare match interrupt
}

