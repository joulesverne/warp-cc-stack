/**
 * @brief A simple application demonstrating how the CC2500 API could be used
 *			in a transmitter.
 *
 * @file demoTransmitter.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0
 */

///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "config/config.h"
#include "hal/hal.h"
#include "hal/bsp.h"
#include "radio/radio.h"
#include "sensor_id.h"

///////////////////////////////////////////////////////////////////////////////
/// Global variables
///////////////////////////////////////////////////////////////////////////////
uint8_t		msgBuf[3];		// Transmit message buffer
uint8_t		calScheduler;	// Calibration schedule tracking
uint16_t	sensorValue;	// ADC result - temporary storage

///////////////////////////////////////////////////////////////////////////////

void main( void )

{
    calScheduler=0;	// Initialize calibration scheduler

    // Initialize microcontroller (Digital and analog I/O, timers, clock, etc)
    HAL_INIT();

    // Initialize board-specific hardware functionality such as sensors,
    //  power management, LEDs, etc.
    BSP_INIT();

    // Load all configuration registers of radio and put radio into idle mode
    RADIO_INIT();

    // Set power level for transmit operation
    RADIO_SET_TX_PWR(0xFF);

    // Radio goes to sleep here to save power during remaining initialization
    RADIO_SLEEP();

    // Initialize the ADC for sensor measurements
    HAL_ADC_INIT();

    // Select the proper input channel for sampling an analog sensor
	HAL_ADC_CHANNEL_SELECT(BSP_INCH_TEMP);
	//HAL_ADC_CHANNEL_SELECT(BSP_INCH_PHOTO);

    while(1)
        {
            // Calibrate radio VCO every fourth frame (starting with first frame)
            if(!calScheduler) {
				// Calibrate the radio frequency synth.
				//	Automatically wakes the radio if it's asleep.
				RADIO_CALIBRATE();
				calScheduler=3;
            } else {
            	calScheduler--;
            }

            // Sample the sensor
            sensorValue = HAL_ADC_SAMPLE();

            // Load up the message buffer
            msgBuf[0] = SENSOR_ID_TEMP;
            msgBuf[2] = sensorValue & 0xFFu; // 8 LSBs
            msgBuf[1] = _swap_bytes(sensorValue) & 0x03u; // 2 MSbs


            // Transmit a message to the receiver. This function will also send
            // 	the network ID and sensor ID automatically.
            // 	This function may return before the transmission is complete.
            //	Automatically wakes the radio if it's asleep.
            RADIO_TX(msgBuf);

            // Add delay to make sure all data is transmitted. What is the minimum delay?
            HAL_PRECISE_DELAY(24);

            RADIO_SLEEP();

            // DEBUG: Delay between packets
            // HAL_LONG_DELAY(1000);
       }

}

///////////////////////////////////////////////////////////////////////////////
