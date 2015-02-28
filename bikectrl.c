/**
* @file bikectrl.c
* @brief Simple bike computer
* @author Ollo
* @version 0.1
*
* 
* Sources:
* - http://winavr.scienceprog.com/avr-gcc-tutorial/interrupt-driven-avr-usart-communication.html
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "uart.h"

/** @fn ISR (TIMER1_OVF_vect)
* @brief Overflow interrupt of the timer
* Logic to stop the motor (release the relais) after a timeout
*/
ISR (TIMER1_OVF_vect)
{

}

/** @fn void timer_init (void)
* @brief activiation of the timer.
* The timer is necessary to trigger a timeout, after which always the motor is stopped.
*/
void timer_init (void)
{
	TIMSK |= (1<<TOIE1); // Timer Overflow Interrupt enable
	TCNT1 = 0; // Rücksetzen des Timers
	TCCR1B = (1<<CS10) | (1<<CS12); // 8MHz/65536/1024 = 0,11Hz --> 8,38s
}

/** @fn int main (void)
* @brief main entry point
* Initializing the hardware IO, the timer and the SPI logic.
*/
int main (void)
{
	//timer_init();
	uart_init();
	stdout = &uart_output;
	stdin = &uart_input;
	//sei ();

	/**** Set ports ****/
	/* Debug output LED */
	DDRB |= _BV(DDB0); /* Pin 14 lower left corner */
	
	char input;

	
	for(;;)
	{
		puts("Hello world!");
		input = getchar();
		printf("You wrote %c\n", input);
		PORTB ^= _BV(PB0);
		_delay_ms(500);

	}
	return 0;
}

