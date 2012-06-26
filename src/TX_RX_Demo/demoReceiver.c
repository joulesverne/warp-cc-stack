/**
 * @brief A simple application demonstrating how the CC2500 API could be used
 *			in a receiver with continuous polling.
 *
 *
 * @file demoReceiver.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0, Jan 2012.
 */

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "hal/hal.h"
#include "hal/bsp.h"
#include "radio/radio.h"

///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////
#define USE_TX_ID	0x77	// The ID of the transmitter we're listening to

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////
static uint8_t rxBuf[RADIO_PAY_LEN]; 			// Receive buffer
static uint8_t newData;

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
void dataReceived();

///////////////////////////////////////////////////////////////////////////////

/*
 * Entry point for demo receiver application
 *
 * @todo Calibrate the radio every once in a while
 * @todo Debug periodic packet drop issues
 * @todo Verify arbitrary-length packet receive mode
 */
void main( void )
{
    /* TODO: Make sure to enable optimizations for proper inlining, etc */

    // Initialize microcontroller (Digital and analog I/O, timers, clock, etc)
    HAL_INIT();

    //Initialize UART communication interface.
    HAL_UART_INIT();

    // Initialize board-specific hardware functionality such as sensors,
    //  power management circuit, supply measurement circuit, LEDs, etc.
    BSP_INIT();

    // Load all configuration registers of radio and put radio into idle mode
    RADIO_INIT();

    RADIO_CALIBRATE();

    RADIO_SETUP_RX(&dataReceived);

    newData = 0;
    // Wait for receive.
    while(1)
        {

            if(newData)
                {

                    if(newData)
                        {
                            BSP_LED0_TOGGLE();
                        }
                }
        }
}


/**
 * A packet has been received. Grab it and process.
 * This runs in an ISR context.
 */
void dataReceived()
{
    uint8_t nwkID;
    uint8_t txID;

    // Toggle LED to indicate that something was received (may not be good data).
    BSP_LED1_TOGGLE();

    // If any data has been received, check the IDs and signal if matching.
    if(RADIO_RECEIVE(rxBuf, &nwkID, &txID))
        {
            if((RADIO_NWK_ID == nwkID) && (USE_TX_ID == txID))
                {
                    HAL_UART_TX(rxBuf,RADIO_PAY_LEN);
                }
        }

    // Disable receive for a moment and recalibrate the radio
    RADIO_IDLE();

    // Calibrate the receiver
    RADIO_CALIBRATE();

    // Re-enable polling
    RADIO_RX_POLL();

}
