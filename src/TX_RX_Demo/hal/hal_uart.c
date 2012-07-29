/**
 * @brief Blocking, power-optimized UART TX functions
 *
 *
 * @file hal_uart.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0
 *
 * @todo Improve the modularity of the UART routines with macros, etc
 */

///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include "hal.h"			// HAL configuration and other HAL functions
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////
/// Local definitions
///////////////////////////////////////////////////////////////////////////////

#define	HAL_UART_ESCAPE_CHAR		0x00
#define	HAL_UART_ESC_NEWFRAME_CHAR	0x01
#define HAL_UART_ESC_ESCAPE_CHAR	0x02

///////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the UART peripheral
 * Uses 9600 8 N 1 (9600 baud, 8 bit frames, No parity, 1 stop bit).
 */
void HAL_UART_INIT( void )
{
    HAL_ENTER_CRITICAL();
    UCA0CTL1 |= UCSWRST;
    // UCA0CTL0 |= UCMSB;				//MSB first
    UCA0CTL1 |= UCSSEL_2;				// SMCLK
    UCA0MCTL = UCBRS0;					// Modulation UCBRSx = 1
    UCA0BR0 = 104;						// Baud rate of 9600 (can change later)
    UCA0BR1 = 0;						// (UCA0BR0 + UCA0BR1 * 256 = UCBR)
    P3SEL |= BIT4 | BIT5;				// P3.4,5 = USCI_A0 TXD/RXD
    UCA0CTL1 &= ~UCSWRST;				// **Initialize USCI state machine**
    HAL_EXIT_CRITICAL();
}


/**
 * Transmits the given string of characters via the UART.
 * Blocks until all characters transmitted.
 *
 * @param msg The string to be transmitted.
 * @param len The length of the string in bytes.
 * @return Always returns HAL_SUCCESS
 *
 * @pre SMCLK assumed to be 1 MHz
 *
 * @todo Use TX interrupts when requested by user
 *
 */
int16_t HAL_UART_TX(uint8_t* msg, uint16_t len)
{
	int i; // Indexing counter

    //Send content of buffer
	for(i=0;i<len;i++)
	{
		// Is the USCI_A0 TX buffer ready?
		while (!(IFG2&UCA0TXIFG));

		// Load the transmit buffer (starts transmit)
		UCA0TXBUF = msg[i];

		while(UCA0STAT&UCBUSY);
	}


	return HAL_SUCCESS;
}

/**
 * Formats data for transmission via a serial link. Escape sequences are used to setup
 * frame headers and other stuff.
 * @param dst	Destination for formatted array with size at least (2*sizeof(src) + 2)
 * @param src	Raw source material
 * @param len	Length of the source array
 * @return	The length of the destination buffer
 */

uint16_t HAL_UART_FORMATTER(uint8_t* dst, uint8_t* src, uint16_t len)
{
	uint16_t s_i; // Index in source buffer
	uint16_t d_i; // Index in destination buffer

	s_i = d_i = 0;

	// Escape sequence for new frame
	dst[d_i++] = HAL_UART_ESCAPE_CHAR;
	dst[d_i++] = HAL_UART_ESC_NEWFRAME_CHAR;

	for(s_i=0;s_i<len;s_i++)
	{
		switch(src[s_i])
		{
			case HAL_UART_ESCAPE_CHAR:
				// Escape sequence required to escape the escape character...
				dst[d_i++] = HAL_UART_ESCAPE_CHAR;
				dst[d_i++] = HAL_UART_ESC_ESCAPE_CHAR;
				break;


			default:
				// Normal characters
				dst[d_i++] = src[s_i];
				break;
		}
	}

	return d_i; // Index has already been post-inc'ed past last char in array.

}
