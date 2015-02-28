/**
* @file bikectrl.c
* @brief Simple bike computer
* @author Ollo
* @version 0.1
*
* 
*
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

	timer_init();
	sei ();
	for (;;)
	{

	}
	return 0;
}

