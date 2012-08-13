/**
 * @brief Board support routines
 *
 * Provides initialization for external peripherals and board features, as
 * well as high-level methods for usage of these features.
 *
 * @file bsp.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include "bsp.h"		// BSP configuration data and referenced functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Initializes all external peripherals and board features
 */
void BSP_INIT( void )
{
	// Run critical power management system init if defined
	POWER_MANAGEMENT_SETUP();

	// Board-specific configuration
	BSP_PIN_SETUP();

	///@todo Add any additional peripheral config here
}


/**
 * Sample the supply voltage using the bandgap reference.
 *
 *	@return The supply voltage. 10 LSBs represent result of ADC10
 *
 * @todo Implement this function
 */
uint16_t BSP_SAMPLE_SUPPLY( void )
{

// 	#ifdef __MSP430F2272__
// 	// Select vrect(output voltage of power harvesting block) as ADC input and enable power supply sensing
// 		BSP_VRECT_EN_POUT |= BSP_VECT_EN;
// 		HAL_ADC_CHANNEL_SELECT(BSP_VRECT_INCH);
// 		BSP_VRECT_EN_POUT &= ~BSP_VECT_EN;
// 	#endif

//	HAL_ADC_CHANNEL_SELECT(BSP_VRECT_INCH);
// 	HAL_ADC_SAMPLE();
//	return ADC10MEM;
    return 0;
}


/**
 * A blocking power management function. Go to sleep, periodically wake
 * and check supply voltage. If supply voltage is over the given threshold,
 * resume program execution.
 *
 * @param vWake the voltage at which to wake, as indexed in bsp.h
 *	@return the amount of time that the function waited for adequate voltage.
 *
 * @todo Implement this function
 */
uint16_t BSP_WAKE_ON_VOLTAGE( uint16_t vWake )
{
//    while(1)
//        {
//            HAL_LONG_DELAY(BSP_SLEEP_TIME);
//            if(BSP_SAMPLE_SUPPLY() > vWake)
//                {
//                    return ADC10MEM;
//                }
//        }
	return 0;
}

