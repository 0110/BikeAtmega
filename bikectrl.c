/**
* @file bikectrl.c
* @brief Simple bike computer
* @author Ollo
* @version 0.1
*
* 
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "uart.h"

#define CENTIMETER_PER_ROTATION	686	/**< The length of the wheel, FIXME: Make this value variable in the flash */

#define PRESCALER_FACTOR	1024	/**< Factore needed to calculate the detected tick-frequency */
#define	HZ2MS			1000	/**< Factore to convert impulse diff to a duration in milliseconds */
#define MAGIC_FACTOR		3600	/**< This is the last number, resulting of the convertion:
					 * - from 1/s to seconds
					 * - m/s to km/h
					 * - length in cm by one rotation of the wheel
					 * - Display 425 for 4.25km/h
					 */

#define DEBOUNCING_VALUE	2755	/**< Ticks, that must be passed to be counted (debouncing) */

volatile unsigned int presses=0;

volatile uint16_t impulse1=0;	/**< Counter value of the timer (necessary to calculate the speed) */
volatile uint16_t impulse2=0;	/**< Counter value of the timer (necessary to calculate the speed) */

/** @fn ISR (TIMER1_OVF_vect)
* @brief Overflow interrupt of the timer
* Set the speed to zero, if there was no tick found
*/
ISR (TIMER1_OVF_vect)
{
	PORTB ^= _BV(PB0); /* Debug Blink-LED */

	/* Reset the ticks */
	presses=0;
	impulse1=0;
	impulse2=0;
}

/** @fn ISR(INT0_vect)
 * Interrupt Service Routine for INT0
 */
ISR(INT0_vect)
{
	/* store the counter value of the last two pulses */
	if ((TCNT1 - impulse2) > DEBOUNCING_VALUE) /* software debouncing */
	{
		impulse1=impulse2;
		impulse2=TCNT1;
		presses++;
	}
}

/** @fn void timer_init (void)
* @brief activiation of the timer.
*/
void timer_init (void)
{
	TIMSK |= (1<<TOIE1); // Timer Overflow Interrupt enable
	TCNT1 = 0; // Reset of the timer

	/* @see PRESCALER_FACTOR */
	TCCR1B = (1<<CS10) | (1<<CS12); /* 8MHz * 1024 * 65536--> 0,11 Between two overruns: 8,38s */
}

uint16_t calculateSpeed()
{
	uint16_t diff = impulse2 - impulse1;
	if (diff == 0)
	{
		return 0;
	}
	uint32_t diffMs = (diff * PRESCALER_FACTOR * HZ2MS) / F_CPU;
	uint16_t km_h = ((CENTIMETER_PER_ROTATION * MAGIC_FACTOR ) / diffMs);
	return diffMs;
}

/** @fn int main (void)
* @brief main entry point
* Initializing the hardware IO, the timer and the SPI logic.
*/
int main (void)
{
	uint16_t speed;
	
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
		speed = calculateSpeed();
		printf("Pressed %3d times last pulses: %5u %5u\tSpeed: %u km/h\n", presses, impulse1, impulse2, speed);
//		input = getchar();
//		printf("You wrote %c\n", input);
		_delay_ms(500);

	}
	return 0;
}

