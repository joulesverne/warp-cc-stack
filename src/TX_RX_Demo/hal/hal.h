/**
 * @brief Provides uC-specific functions for control of hardware and peripherals
 *
 * Hardware Abstraction Layer for the ultra-low-power networking stack. This
 * provides functionality that is specific to a particular microcontroller type.
 *
 * @file hal.h
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

/*---------------------Include Guard-----------------------------------------*/
#ifndef HAL_H
#define HAL_H
/*---------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include "../config/config.h"


///////////////////////////////////////////////////////////////////////////////
/// Return status definitions
///////////////////////////////////////////////////////////////////////////////

#define HAL_SUCCESS 		0	// Successful completion of function
#define HAL_FAIL			1	// Non-specific failure occurred
#define TRUE				1	// Compiler should interpret as true
#define	FALSE				0	// Compiler should interpret as false


///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include <stdint.h>		// Data type definitions

///////////////////////////////////////////////////////////////////////////////
/// Peripheral definitions
///////////////////////////////////////////////////////////////////////////////

// SPI peripheral
#define HAL_SPI_TXBUF	UCB0TXBUF
#define HAL_SPI_RXBUF	UCB0RXBUF
#define HAL_SPI_TX_VECTOR USCIAB0TX_VECTOR
#define HAL_SPI_RX_VECTOR USCIAB0RX_VECTOR

// Timer(s)
#define HAL_TMR_VECTOR	TIMERA0_VECTOR
//TimerB vector
#define HAL_TMB_VECTOR  TIMERB0_VECTOR


///////////////////////////////////////////////////////////////////////////////
/// Prototypes and macros
///////////////////////////////////////////////////////////////////////////////

//-------General initialization routine--------------------------------------//
void HAL_INIT( void );

//-------Enable/disable interrupts-------------------------------------------//
#define HAL_ENABLE_INTERRUPTS()		_bis_SR_register(GIE)
#define HAL_DISABLE_INTERRUPTS()	_bic_SR_register(GIE)

//-------Critical section enter/exit-----------------------------------------//
/// @todo Add state memory between ENTER() and EXIT() calls.
#define HAL_ENTER_CRITICAL()		HAL_DISABLE_INTERRUPTS()
#define HAL_EXIT_CRITICAL()			HAL_ENABLE_INTERRUPTS()

//-------Low-power mode enter/exit-------------------------------------------//
#define HAL_SLEEP()					__bis_SR_register(LPM3_bits | GIE)
#define HAL_LPM3_WAKEUP()			__bic_SR_register_on_exit(LPM3_bits | GIE)
#define HAL_LPM1_SLEEP()			__bis_SR_register(LPM1_bits | GIE)
#define HAL_LPM1_WAKEUP()			__bic_SR_register_on_exit(LPM1_bits | GIE)
#define HAL_SLEEP_OSC_OFF()			__bis_SR_register(LPM4_bits | GIE)
#define HAL_WAKE_ON_ISR_EXIT()		__bic_SR_register_on_exit(LPM4_bits)


//-------Power-optimized hardware sleep functions----------------------------//

// Short delay; Blocks for the given number of microseconds assuming 1MHz clk
#define HAL_DELAY_SHORT( us ) __delay_cycles(us)

// Delay for longer periods of time, using watch crystal and LPM3.
void HAL_PRECISE_DELAY(uint16_t ticks);
void HAL_LONG_DELAY(uint16_t ticks);

//-------ADC module functions------------------------------------------------//

// ADC initialization
void HAL_ADC_INIT( void );
// Switch ADC channel
void HAL_ADC_CHANNEL_SELECT( int channel );
// Poll ADC channel
uint16_t HAL_ADC_SAMPLE( void );

//-------SPI module functions------------------------------------------------//

// SPI initialization
void HAL_SPI_INIT( void );
// SPI receive
uint8_t HAL_SPI_READ (
		uint8_t addr,
		uint8_t* rxBytePtr,
		uint8_t len,
		uint16_t dly
		);

// SPI multi-byte block write
uint8_t HAL_SPI_WRITE (
		uint8_t addr,
		uint8_t* txBufPtr,
		uint8_t len,
		uint16_t dly
		);

// SPI Strobe
uint8_t HAL_SPI_STROBE(uint8_t strobeCmd, uint16_t dly);

//-------UART module functions-----------------------------------------------//

// UART initialization
void HAL_UART_INIT( void );
// UART transmit functions
int16_t HAL_UART_TX(uint8_t* msg, uint16_t len);


///////////////////////////////////////////////////////////////////////////////
#endif /* HAL_H */
///////////////////////////////////////////////////////////////////////////////
