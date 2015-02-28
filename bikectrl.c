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
* Set the speed to zero, if there was no tick found
*/
ISR (TIMER1_OVF_vect)
{

}

volatile unsigned int presses=0;

/** @fn ISR(INT0_vect)
 * Interrupt Service Routine for INT0
 */
ISR(INT0_vect)
{
	PORTB ^= _BV(PB0);
	presses++;
}

/** @fn void timer_init (void)
* @brief activiation of the timer.
*/
void timer_init (void)
{
	TIMSK |= (1<<TOIE1); // Timer Overflow Interrupt enable
	TCNT1 = 0; // Reset of the timer
	TCCR1B = (1<<CS10) | (1<<CS12); // 8MHz/65536/1024 = 0,11Hz --> 8,38s
}

/** @fn int main (void)
* @brief main entry point
* Initializing the hardware IO, the timer and the SPI logic.
*/
int main (void)
{
	/* Initialize the Timer */
	timer_init();
	/* Init the serial interface */
	uart_init();
	stdout = &uart_output;
	stdin = &uart_input;
	/* Initialize Interrupt input */
	DDRD = 1<<PD2;		// Set PD2 as input (Using for interupt INT0)
	PORTD = 1<<PD2;		// Enable PD2 pull-up resistor
	GICR = 1<<INT0;                 // Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge

	sei ();

	for(;;)
	{
		printf("Pressed %d times\n", presses);
//		input = getchar();
//		printf("You wrote %c\n", input);
		_delay_ms(500);

	}
	return 0;
}

