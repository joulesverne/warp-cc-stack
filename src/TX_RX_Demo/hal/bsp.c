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
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "bsp.h"		// BSP configuration data and referenced functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Initializes all external peripherals and board features
 *
 * @todo SPI/LEDs implemented. Add additional needed port config settings.
 * @todo Test SPI/LED port control
 * @todo Compress this into one assignment per register.
 */
void BSP_INIT( void )
{
    // LEDs
    BSP_LED_POUT &= ~(BSP_LED_0_BIT | BSP_LED_1_BIT);
    BSP_LED_PDIR |= BSP_LED_0_BIT | BSP_LED_1_BIT;

    // SPI
    BSP_SPI_POUT 	&= 	~(BSP_SPI_SIMO_BIT | BSP_SPI_CLK_BIT | BSP_SPI_CS_BIT);
    //DEBUG: The code below would force to turn on the radio, this should only be done when needed.
    //BSP_SPI_POUT 	|=	BSP_SPI_CS_BIT;
    BSP_SPI_PDIR 	|= 	BSP_SPI_CS_BIT;
    BSP_SPI_PSEL	|=	BSP_SPI_SIMO_BIT | BSP_SPI_SOMI_BIT | BSP_SPI_CLK_BIT;

    // GDO
    BSP_GDO_PSEL	&= ~(BSP_GDO0_BIT | BSP_GDO2_BIT);
    BSP_GDO_PDIR	&= ~(BSP_GDO0_BIT | BSP_GDO2_BIT);

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
 * A blocking power management functino. Go to sleep, periodically wake
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
    while(1)
        {
            HAL_LONG_DELAY(BSP_SLEEP_TIME);
            if(BSP_SAMPLE_SUPPLY() > vWake)
                {
                    return ADC10MEM;
                }
        }
}

