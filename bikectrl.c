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

#define BAUDRATE 9600
//calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

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

/** @fn ISR(USART1_RXC_vect)
 *
 */
ISR(USART1_RX_vect)
{
	//defien temp value for storing received byte
	uint8_t Temp;

	//Store data to temp
	Temp=UDR;
	Temp++;//increment

	//send received data back

	// no need to wait for empty send buffer
	UDR=Temp;
}

/** @fn void usart_init(void)
 * @brief initialize the UART
 */
void usart_init(void)
{
	//Set baud rate
	UBRRL=UBRRVAL;		//low byte
	UBRRH=(UBRRVAL>>8);	//high byte

	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
		(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);	

	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);

	//enable global interrupts
	set_sleep_mode(SLEEP_MODE_IDLE);
}

/** @fn int main (void)
* @brief main entry point
* Initializing the hardware IO, the timer and the SPI logic.
*/
int main (void)
{
	usart_init();
	//timer_init();
	sei ();
	
	for(;;)
	{
		sleep_mode();
	}
	return 0;
}

