/**
 * @brief Platform-specific IO assignments and other info for the WARP rev1
 * 			platform
 *
 *
 * @file config_warp_rev1
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0
 */

/*---------------------Include Guard-----------------------------------------*/
#ifndef CONFIG_PLATFORM_SPECIFIC
#define CONFIG_PLATFORM_SPECIFIC
/*---------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
///	Includes
///////////////////////////////////////////////////////////////////////////////
#include <msp430f2274.h>

///////////////////////////////////////////////////////////////////////////////
///	Board-specific IO definitions (GPIO, ADC, timers, etc)
///////////////////////////////////////////////////////////////////////////////

// GDO Port(s)
#define BSP_GDO_PDIR			P1DIR
#define BSP_GDO_PSEL			P1SEL
#define BSP_GDO_POUT			P1OUT
#define BSP_GDO_PIN				P1IN
#define BSP_GDO_PIE				P1IE
#define BSP_GDO_PIFG			P1IFG
#define BSP_GDO_PIES			P1IES

// GDO Pins
#define BSP_GDO0_BIT			BIT2
#define BSP_GDO2_BIT			BIT3

// GDO Port interrupt vector
#define BSP_GDO_VECTOR			PORT1_VECTOR

// LED Port
#define BSP_LED_PDIR			P1DIR
#define BSP_LED_POUT			P1OUT // Port with LED lines

// LED Pins
#define BSP_LED_0_BIT			BIT0
#define BSP_LED_1_BIT			BIT1

// SPI Port
#define BSP_SPI_PDIR			P3DIR
#define BSP_SPI_PSEL			P3SEL
#define BSP_SPI_POUT			P3OUT
#define BSP_SPI_PIN				P3IN

// SPI Pins
#define BSP_SPI_CS_BIT			BIT0
#define BSP_SPI_SIMO_BIT		BIT1
#define BSP_SPI_SOMI_BIT		BIT2
#define BSP_SPI_CLK_BIT			BIT3

// Power Management Pins
#define BSP_LDO_HOLD_PDIR		P4DIR
#define BSP_LDO_HOLD_POUT		P4OUT
#define BSP_LDO_HOLD_BIT		BIT4

// Photosensor
#define BSP_PHOTO_PWR_POUT		P1OUT
#define BSP_PHOTO_PWR_PDIR		P1DIR
#define BSP_PHOTO_PWR_BIT		BIT6
#define BSP_PHOTO_ADC_PDIR		P3DIR
#define BSP_PHOTO_ADC_BIT		BIT7


// Watch crystal
#define BSP_HAS_XTAL			0
//#define BSP_XTAL_PSEL			P1SEL
//#define BSP_XIN				BIT0
//#define BSP_XOUT				BIT1

///////////////////////////////////////////////////////////////////////////////
/// IO Setup macros
///////////////////////////////////////////////////////////////////////////////
#define POWER_MANAGEMENT_SETUP() \
			BSP_LDO_HOLD_POUT |= BSP_LDO_HOLD_BIT;\
			BSP_LDO_HOLD_PDIR |= BSP_LDO_HOLD_BIT;

#define	BSP_PIN_SETUP() \
			BSP_LED_POUT &= ~(BSP_LED_0_BIT | BSP_LED_1_BIT);\
			BSP_LED_PDIR |= BSP_LED_0_BIT | BSP_LED_1_BIT;\
			BSP_SPI_POUT 	&= 	~(BSP_SPI_SIMO_BIT | BSP_SPI_CLK_BIT | BSP_SPI_CS_BIT);\
			BSP_SPI_PDIR 	|= 	BSP_SPI_CS_BIT;\
			BSP_SPI_PSEL	|=	BSP_SPI_SIMO_BIT | BSP_SPI_SOMI_BIT | BSP_SPI_CLK_BIT;\
			BSP_GDO_PSEL	&= ~(BSP_GDO0_BIT | BSP_GDO2_BIT);\
			BSP_GDO_PDIR	&= ~(BSP_GDO0_BIT | BSP_GDO2_BIT);\
			BSP_PHOTO_PWR_POUT &= ~(BSP_PHOTO_PWR_BIT);\
			BSP_PHOTO_PWR_PDIR |= (BSP_PHOTO_PWR_BIT);\
			BSP_PHOTO_ADC_PDIR &= ~(BSP_PHOTO_ADC_BIT)

#define BSP_PHOTO_ENABLE() \
			BSP_PHOTO_PWR_POUT |= BSP_PHOTO_PWR_BIT

#define BSP_PHOTO_DISABLE() \
			BSP_PHOTO_PWR_POUT &= ~(BSP_PHOTO_PWR_BIT)

///////////////////////////////////////////////////////////////////////////////
/// ADC Supply Sensing Result conversions
/// @todo Fill in supply voltage conversion values for this platform
///////////////////////////////////////////////////////////////////////////////
/*
#define BSP_SUPPLY_V_20			0
#define BSP_SUPPLY_V_21			0
#define BSP_SUPPLY_V_22			0
#define BSP_SUPPLY_V_23			0
#define BSP_SUPPLY_V_24			0
#define BSP_SUPPLY_V_25			0
#define BSP_SUPPLY_V_26			0
#define BSP_SUPPLY_V_27			0
*/


///////////////////////////////////////////////////////////////////////////////
/// Global defines for customized application
///////////////////////////////////////////////////////////////////////////////
//#define BSP_WAKEUP_V		 1.9					//wake up voltage from sleep moed
//#define BSP_WAKEUP_VSENSE 	((BSP_WAKEUP_V-0.8)/3)  //Actually voltage sensed by ADC10
//#define BSP_SUPPLY_V 		128//1023*BSP_WAKEUP_VSENSE/1.5
//#define BSP_SLEEP_TIME 		10000 // cycles for board to check power supply in sleep mode

///////////////////////////////////////////////////////////////////////////////
#endif /* CONFIG_PLATFORM_SPECIFIC */
///////////////////////////////////////////////////////////////////////////////
