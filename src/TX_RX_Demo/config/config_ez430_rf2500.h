/**
 * @brief Platform-specific IO assignments and other info for the ez430-RF2500
 *
 *
 * @file config_ez430_rf2500.h
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
#define BSP_GDO_PDIR			P2DIR
#define BSP_GDO_PSEL			P2SEL
#define BSP_GDO_POUT			P2OUT
#define BSP_GDO_PIN				P2IN
#define BSP_GDO_PIE				P2IE
#define BSP_GDO_PIFG			P2IFG
#define BSP_GDO_PIES			P2IES

// GDO Pins
#define BSP_GDO0_BIT			BIT6
#define BSP_GDO2_BIT			BIT7

// GDO Port interrupt vector
#define BSP_GDO_VECTOR			PORT2_VECTOR

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

// Watch crystal
#define BSP_HAS_XTAL			0
//#define BSP_XTAL_PSEL			P1SEL
//#define BSP_XIN					BIT0
//#define BSP_XOUT				BIT1

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
#define BSP_WAKEUP_VSENSE 	((BSP_WAKEUP_V-0.8)/3)  //Actually voltage sensed by ADC10
#define BSP_SUPPLY_V 		128//1023*BSP_WAKEUP_VSENSE/1.5
#define BSP_INCH 1			//Sensor input channel for ADC input
#define BSP_SLEEP_TIME 10000 // cycles for board to check power supply in sleep mode

///////////////////////////////////////////////////////////////////////////////
#endif /* CONFIG_PLATFORM_SPECIFIC */
///////////////////////////////////////////////////////////////////////////////
