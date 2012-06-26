/**
 * @brief General HAL functionality
 *
 * This file provides initialization, interrupt enable/disable, power mode
 * control, etc.
 *
 * @file hal.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "hal.h"			// HAL configuration and other HAL functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Initializes the microcontroller, including digital & analog I/O, timers,
 *	and any other built-in peripherals.
 *
 * @todo Implement any other initialization needed
 */
void HAL_INIT( void )
{
    // Stop the WDT
    WDTCTL = WDTPW | WDTHOLD;

    // Set up the clock for 1 MHz
    // @todo Try various clock frequencies...
    BCSCTL1 |= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

#if(BSP_HAS_CRYSTAL)
    //Choose 32.768kHz watch crystal as AMCLK source
    BSP_XTAL_PSEL |= BSP_XOUT + BSP_XIN;		// Select XT1
    BCSCTL3 |= XCAP_3;                       	// Internal load cap
    // Select ACLK = VLO
#else
    BCSCTL3 |= LFXT1S_2; //use VLO as clock source
#endif

    // @todo Setup the crystal capacitance regs
    // @todo Initialize the delay timer peripheral
    HAL_SPI_INIT();
    //HAL_UART_INIT();
    HAL_ENABLE_INTERRUPTS();
}

/**
 * If HAL_OPTIMIZE_INIT is non-zero...
 * These two functions (_system_pre_init and _auto_init) replace the compiler-
 * generated system pre-initialization routines in CCS.
 *
 * WARNING: Do not expect any global initialization to occur if using this function.
 */
#if(HAL_OPTIMIZE_INIT)

void _system_pre_init( void )
{
    // Do nothing
}

void _auto_init( void )
{
    // Do nothing
}

#endif
///////////////////////////////////////////////////////////////////////////////
