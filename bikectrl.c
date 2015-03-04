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

volatile unsigned int presses=0;

volatile unsigned int impulse1=0;
volatile unsigned int impulse2=0;

/** @fn ISR (TIMER1_OVF_vect)
* @brief Overflow interrupt of the timer
* Set the speed to zero, if there was no tick found
*/
ISR (TIMER1_OVF_vect)
{
	PORTB ^= _BV(PB0); /* Debug Blink-LED */

	/* Reset the ticks */
	presses=0;
}

/** @fn ISR(INT0_vect)
 * Interrupt Service Routine for INT0
 */
ISR(INT0_vect)
{
	presses++;

	/* store the counter value of the last two pulses */
	impulse1=impulse2;
	impulse2=TCNT1;
}

/** @fn void timer_init (void)
* @brief activiation of the timer.
*/
void timer_init (void)
{
	TIMSK |= (1<<TOIE1); // Timer Overflow Interrupt enable
	TCNT1 = 0; // Reset of the timer
	TCCR1B = (1<<CS12); /* 8MHz * 256 * 65536--> 0,11 Between two overruns: 2,09s */
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
	DDRD = _BV(PD2);		// Set PD2 as input (Using for interupt INT0)
	PORTD = _BV(PD2);		// Enable PD2 pull-up resistor
	GICR = _BV(INT0);                 // Enable INT0
	MCUCR = _BV(ISC01) | _BV(ISC00);	// Trigger INT0 on rising edge

	/* Debug output LED */
	DDRB |= _BV(DDB0); /* Pin 14 lower left corner */

	sei ();

	printf("Booted\n");
	for(;;)
	{
		printf("Pressed %3d times last pulses: %3u, %3u\n", presses, impulse1, impulse2);
//		input = getchar();
//		printf("You wrote %c\n", input);
		_delay_ms(500);

	}
	return 0;
}

