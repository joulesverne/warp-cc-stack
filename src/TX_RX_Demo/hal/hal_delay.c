/**
 * @brief Power-optimized time delay functions.
 * @file hal_delay.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0
 */

///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include "hal.h"			// HAL configuration and other HAL functions
///////////////////////////////////////////////////////////////////////////////
/**
 * Long delay; Blocks for the given number of ticks of a 32,768 Hz
 * RTC crystal. Good for longer delays where high precision timing
 * is required.
 *
 * @param ticks ticks/32,768 = time delay in seconds.
 *
 * @todo Use watch crystal and low-power mode (or use ACLK as source clock)
 */
void HAL_PRECISE_DELAY(uint16_t ticks)
{
    int i;

    if(!ticks)
        {
            return;
        }

    // @todo: only use this method if there's no crystal
    for(i = 0; i < ticks; i++)
        {
            HAL_DELAY_SHORT( 31 ); // Emulate one watch crystal tick (30.5 us)
        }



}

/**
 * Wait for the specified period of time using the VLO (Very Low-freq Oscillator).
 * This function should not be used in an ISR context. Use this for longer
 * delays where low precision timing is required.
 *
 * @param ticks Number of clock ticks of the VLO (about 12kHz) to wait
 *
 */
//----------------------------------------------------------------------------------
void HAL_LONG_DELAY(uint16_t ticks)
{
    // Zero the timing counter
    TACTL = TACLR;
    TACTL = TASSEL_1 + MC_1;

    // Configure timer threshold
    TACCR0 = ticks;

    // Enable interrupting on CCR0 threshold
    TACCTL0 |= CCIE;

    // Go into low power mode without disabling oscillator
    HAL_SLEEP();
}

/**
 * General timer ISR; Wakes the CPU.
 */
#pragma vector=HAL_TMR_VECTOR
__interrupt void HAL_TMR_ISR( void )
{
    //TX interrupt routine
    TACCTL0 &= ~CCIE;
    TACTL = MC_0 + TACLR;
    HAL_LPM3_WAKEUP();
}
///////////////////////////////////////////////////////////////////////////////
