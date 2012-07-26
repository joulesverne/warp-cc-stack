/**
 * @brief Blocking, power-optimized SPI TX/RX functions
 *
 * Only needs to support one SPI slave device.
 *
 * @file hal_spi.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include "hal.h"			// HAL configuration and other HAL functions

///////////////////////////////////////////////////////////////////////////////
/// Local prototypes
///////////////////////////////////////////////////////////////////////////////

// Send byte and wait for TX to finish
inline void HAL_SPI_TX_WAIT( uint8_t txByte );

// ~CS line LOW to start transaction
inline void HAL_SPI_CSN_HI();

// ~CS line back HIGH to end transaction
inline void HAL_SPI_CSN_LO( uint16_t dly );

///////////////////////////////////////////////////////////////////////////////

/**
 * Initializes USCIB0 as an SPI Master interface with baud rate equal to
 * undivided SMCLK.
 *
 */
void HAL_SPI_INIT( void )
{
    // Enter a critical section (disable interrupts)
    HAL_ENTER_CRITICAL();

    // Place USCIB0 interface in Reset mode during configuration
    // @todo is this actually guaranteed on reset?
    // UCB0CTL1 = UCSWRST;


    UCB0CTL1 |= UCSWRST;
    // 8-bit SPI Master 3-pin mode
    UCB0CTL0  = UCCKPH + UCMSB + UCMST + UCSYNC;

    // Select clock source (SMCLK)
    UCB0CTL1 |= UCSSEL_2;

    // Just use clock source with no division. Lowest "stable" bitrate is 2.
    // @TODO: Experiment with different baud rates / clock rates here.
    UCB0BR0 = 0x01;
    UCB0BR1 = 0x00;

    // Release for operation
    UCB0CTL1 &= ~UCSWRST;

    //@todo Make sure the USCI is NOT consuming power right now
    // Enable SPI TX/RX interrupt to allow sleep during TX.
    // This interrupt configuration must after the UCSWRST clear
    //IFG2 &= ~(UCB0TXIFG | UCB0RXIFG);
    // IE2 = UCB0TXIE | UCB0RXIE;

    // Exit the critical section (reenable interrupts)
    HAL_EXIT_CRITICAL();
}


/**
 * Reads the given number of characters from the given location of the
 * connected SPI slave device.
 *
 * @param 	addr 		The address from which to read
 * @param 	len 		The number of bytes to read
 * @param	rxBytePtr 	Pointer to the receive array
 * @param 	dly			The number of ticks to wait after lowering ~CS line
 * @return 	The return code from the first SPI transaction
 *
 */
uint8_t HAL_SPI_READ(uint8_t addr, uint8_t* rxBytePtr, uint8_t len, uint16_t dly)
{
    uint8_t i;
    uint8_t rc;

    HAL_ENTER_CRITICAL();

    // Pull ~CS line LO to start transaction
    HAL_SPI_CSN_LO( dly );

    // Send the address and wait until transmission is complete
    HAL_SPI_TX_WAIT(addr);

    // Get return code (typically status byte)
    rc = HAL_SPI_RXBUF;

    for (i = 0; i < len; i++)
        {
            HAL_SPI_TX_WAIT(addr);

            // Store data from last data RX
            rxBytePtr[i] = 	HAL_SPI_RXBUF;
        }

    // Pull ~CS line HI to end transaction
    HAL_SPI_CSN_HI();

    HAL_EXIT_CRITICAL();

    return rc;
}

/**
 * Transmits the given string of characters via the SPI port.
 * Blocks until all characters transmitted.
 *
 * @param addr The first target memory address for the string.
 * @param txBufPtr A pointer to the first character of the string.
 * @param len		The length of the string to transmit, in bytes.
 * @param dly		The number of ticks to wait after lowering ~CS line
 *	@return The return code from the first SPI transaction
 *
 */
uint8_t HAL_SPI_WRITE(uint8_t addr, uint8_t* txBufPtr, uint8_t len, uint16_t dly)
{
    uint8_t i;
    uint8_t rc;

    HAL_ENTER_CRITICAL();

    HAL_SPI_CSN_LO( dly );	// Pull ~CS line LO to start transaction

    HAL_SPI_TX_WAIT(addr); // Send write address and wait until transmission is complete

    rc = HAL_SPI_RXBUF; // Get return code (typically status byte)

    for(i = 0; i < len; i++)
        {
            HAL_SPI_TX_WAIT(txBufPtr[i]); // Send the next byte and wait until tx complete
        }

    HAL_SPI_CSN_HI();	// Pull ~CS line HI to end transaction

    HAL_EXIT_CRITICAL();

    return rc;
}

/**
 * Sends the given strobe command to the device on the SPI bus
 *
 * @param strobeCmd The 8-bit strobe command
 * @param dly		The number of ticks to wait after lowering ~CS line
 *
 * @return The return code from the first SPI transaction
 *
 */
uint8_t HAL_SPI_STROBE(uint8_t strobeCmd, uint16_t dly)
{
    HAL_ENTER_CRITICAL();

    HAL_SPI_CSN_LO( dly );	// Pull ~CS line LO to start transaction

    HAL_SPI_TX_WAIT(strobeCmd); // Send the next byte and wait until tx complete

    HAL_SPI_CSN_HI();

    HAL_EXIT_CRITICAL();

    return HAL_SPI_RXBUF; // pass back the return code (typically status byte)
}

/**
 * Send the given byte and block until the transmission has been completed.
 * Assumes ~CS line is already LOW (transaction is in progress).
 *
 * @param txByte The byte to send
 *
 * @todo Use interrupts here
 */
void HAL_SPI_TX_WAIT( uint8_t txByte )
{
    HAL_SPI_TXBUF = txByte;

    while(UCB0STAT & UCBUSY);

    //HAL_LPM1_SLEEP();
}

/**
 * Pull the ~CS line LOW to start a transaction, and delay if needed.
 * @param dly		The number of ticks to wait after lowering ~CS line
 */
void HAL_SPI_CSN_LO( uint16_t dly )
{
    BSP_SPI_POUT &= ~ BSP_SPI_CS_BIT; // ~CS line LO

    // Wait for device to respond
    while(BSP_SPI_PIN & BSP_SPI_SOMI_BIT);

    // @todo Check if the following is necessary after waiting on SOMI value.
    if(dly)
        {
            HAL_PRECISE_DELAY( dly ); // Wait dly ticks of 32Hz clock (30.5us)
        }
}

/**
 * Put the ~CS line back high after transaction
 *
 */
void HAL_SPI_CSN_HI( void )
{
    BSP_SPI_POUT |= BSP_SPI_CS_BIT; // Pull ~CS line HI to end transaction
}
