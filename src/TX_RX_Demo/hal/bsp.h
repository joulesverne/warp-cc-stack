/**
 * @brief Provides board-specific functions for peripheral control
 *
 * Board Support Package (BSP) for the ultra-low-power networking stack.
 * This is uC-independent, and only includes functionality specific to the PCB.
 *
 * @file bsp.h
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

/*---------------------Include Guard-----------------------------------------*/
#ifndef BSP_H
#define BSP_H
/*--------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "../config/config.h"


///////////////////////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////////////////////

/**
 * Board-level peripheral control macros
 */
#define BSP_LED0_ON()		BSP_LED_POUT |= BSP_LED_0_BIT
#define BSP_LED0_OFF()		BSP_LED_POUT &= ~BSP_LED_0_BIT
#define BSP_LED0_TOGGLE()	BSP_LED_POUT ^= BSP_LED_0_BIT
#define BSP_LED1_ON()		BSP_LED_POUT |= BSP_LED_1_BIT
#define BSP_LED1_OFF()		BSP_LED_POUT &= ~BSP_LED_1_BIT
#define BSP_LED1_TOGGLE()	BSP_LED_POUT ^= BSP_LED_1_BIT

///////////////////////////////////////////////////////////////////////////////
// Return status definitions
///////////////////////////////////////////////////////////////////////////////
#define BSP_SUCCESS 0	// Successful completion of function
#define BSP_FAIL	1	// Non-specific failure occurred

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "hal.h" 			// Hardware abstraction layer functions
#include <stdint.h>		// Data type definitions

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

// Initialize board-specific functionality (LEDs, sensing & measurement, etc).
void BSP_INIT( void );

// Sample the supply voltage (units?)
uint16_t BSP_SAMPLE_SUPPLY();
// Sleep until voltage is over given threshold

uint16_t BSP_WAKE_ON_VOLTAGE( uint16_t vWake );



///////////////////////////////////////////////////////////////////////////////
#endif /* BSP_H */
///////////////////////////////////////////////////////////////////////////////
