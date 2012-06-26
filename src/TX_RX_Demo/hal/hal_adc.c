/**
 * @brief Power optimized ADC read functions
 *
 *
 *
 * @file hal_adc.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "hal.h"			// HAL configuration and other HAL functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Look-up tables for ADC channel selection
 */
uint16_t HAL_ADC_CHAN_TABLE_CTL[] =
{
    INCH_0, INCH_1, INCH_2, INCH_3, INCH_4, INCH_5, INCH_6, INCH_7, INCH_8,
    INCH_9, INCH_10, INCH_11, INCH_12, INCH_13, INCH_14, INCH_15
};
uint16_t HAL_ADC_CHAN_TABLE_AE0[] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

/**
 * Initialize the ADC, but do not turn it on.
 */
void HAL_ADC_INIT( void )
{
    ADC10CTL0 |= SREF_1 + ADC10SHT_1 +ADC10IE + ADC10SR;//0-2.5 reference, reduce reference buffer power cost
    ADC10CTL1 |= ADC10SSEL_1; // Single Chanel, ACLK
    ADC10CTL0 &=~(ENC + ADC10SC);
}

/**
 * Select a channel by index.
 *
 * @param channel the channel index to use (A0 - A15?)
 */
void HAL_ADC_CHANNEL_SELECT(int channel)
{
    // Select channel using flash LUT
    //	Uses values: ADC10SSEL_1+(channel)*0x1000u;
    ADC10CTL1 = HAL_ADC_CHAN_TABLE_CTL[channel];

    if(channel < 9)
        {
            //Uses values: ADC10AE0 = 0x1<<channel;
            ADC10AE0 =	HAL_ADC_CHAN_TABLE_AE0[channel];
        }
}

/**
 * Sample the currently selected ADC channel.
 *
 * @return the sampled signal. The 10 LSBs represent ADC10 result.
 */
uint16_t HAL_ADC_SAMPLE( void )
{
    ADC10CTL0 |= REFON + ADC10ON;

    // Delay at lease 30us to allow voltage reference to settle
    HAL_PRECISE_DELAY(1);

    ADC10CTL0 |= ENC + ADC10SC;
    HAL_SLEEP();
    return ADC10MEM;
}

///////////////////////////////////////////////////////////////////////////////
// ADC10 interrupt service routine
///////////////////////////////////////////////////////////////////////////////
#pragma vector=ADC10_VECTOR
__interrupt void Check_power_ISR (void)
{
    ADC10CTL0 &=~(ENC + ADC10SC);
    ADC10CTL0 &=~(REFON + ADC10ON);
    HAL_LPM3_WAKEUP();
}
