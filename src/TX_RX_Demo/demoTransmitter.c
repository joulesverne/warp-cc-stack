/**
 * @brief A simple application demonstrating how the CC2500 API could be used
 *			in a transmitter.
 *
 * @file demoTransmitter.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0
 */

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "config/config_ez430_rf2500.h"
#include "hal/hal.h"
#include "hal/bsp.h"
#include "radio/radio.h"
///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
volatile uint16_t sensorValue;
///////////////////////////////////////////////////////////////////////////////

volatile uint8_t debugRegCheck[64];
volatile uint8_t statusByte;
volatile uint8_t marcState[32];
void main( void )

{
    uint8_t txString[2];	// Formatted string to send as packet payload
    uint16_t sensorValue;	// Temporary storage for sensor value
    uint8_t i=0;			// Periodic calibration counter

    // Initialize microcontroller (Digital and analog I/O, timers, clock, etc)
    HAL_INIT();

    // Initialize board-specific hardware functionality such as sensors,
    //  power management, LEDs, etc.
    BSP_INIT();

    // Load all configuration registers of radio and put radio into idle mode
    RADIO_INIT();

    // Set power level for transmit operation
    RADIO_SET_TX_PWR(0x81); //dBm?
    RADIO_SLEEP();

    // Initialize the ADC for sensor measurements
    HAL_ADC_INIT();

    // Select the proper input channel for sampling an analog sensor
    HAL_ADC_CHANNEL_SELECT(BSP_INCH);

    while(1)
        {
            // Calibrate radio VCO every fourth frame (starting with first frame)
            if(!i)
                {
                    RADIO_CALIBRATE();
                    i=3;
                }
            else
                {
                    i--;
                }

            // Transmit a message to the receiver. This function will also send
            // 	the network ID and sensor ID automatically.
            // 	This function will block until the transmission is complete.

            RADIO_TX("Hello\n");

            // DEBUG: Check status byte to determine what radio is doing right now
            // statusByte = HAL_SPI_STROBE(CC2500_WRITE_SINGLE, 0);
            // DEBUG: Check marcState
            // HAL_SPI_READ(CC2500_MARCSTATE | CC2500_READ_BURST, (uint8_t*)(&marcState), 1, 0);

            // Add delay to make sure all data is transmitted. What is the minimum delay?
            HAL_PRECISE_DELAY(24);

            RADIO_SLEEP();

            // Delay for a while between packets
            HAL_LONG_DELAY(100);

        }

}

///////////////////////////////////////////////////////////////////////////////
