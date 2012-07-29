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
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "hal/hal.h"
#include "hal/bsp.h"
#include "radio/radio.h"
#include "sensor_id.h"

///////////////////////////////////////////////////////////////////////////////
/// Defines
///////////////////////////////////////////////////////////////////////////////
#define USE_TX_ID	0x77	// The ID of the transmitter we're listening to

///////////////////////////////////////////////////////////////////////////////
/// Globals
///////////////////////////////////////////////////////////////////////////////
uint8_t rxBuf[RADIO_PAY_LEN]; 					// Receive buffer
uint8_t msgBuf[2*RADIO_PAY_LEN + 2];			// UART message buffer
uint16_t msgLen;								// Length of UART message
uint8_t calibrateSem;


///////////////////////////////////////////////////////////////////////////////
/// Prototypes
///////////////////////////////////////////////////////////////////////////////
void dataReceived();
void calibrateAndRestartRX(uint8_t* inUse);
uint16_t badBitCount(uint8_t* str1, uint8_t* str2, uint16_t len);

///////////////////////////////////////////////////////////////////////////////

/**
 * Entry point for demo receiver application
 *
 * @todo Debug periodic packet drop issues
 * @todo Verify arbitrary-length packet receive mode
 */
void main( void )
{
    /* TODO: Make sure to enable optimizations for proper inlining, etc */

	// Signal that calibration routine is not in use
	calibrateSem = 0;

	// Initialize microcontroller (Digital and analog I/O, timers, clock, etc)
    HAL_INIT();

    // Initialize board-specific hardware functionality such as sensors,
    //  power management circuit, supply measurement circuit, LEDs, etc.
    BSP_INIT();

    //Initialize UART communication interface.
    HAL_UART_INIT();

    // Load all configuration registers of radio and put radio into idle mode
	RADIO_INIT();

    RADIO_CALIBRATE();

    RADIO_SETUP_RX(&dataReceived);

    // WDT Throws interrupt every ACLK/32,768 seconds
    WDTCTL = WDT_ADLY_1000 | WDTPW;
    IFG1 &= ~WDTIFG;
    IE1 |= WDTIE;

    // Wait for receive.
    while(1);
}


/**
 * A packet has been received. Grab it and process. This runs in an ISR context.
 *  Do not enable any radio-related interrupts in this callback function.
 */
void dataReceived()
{
    uint8_t nwkID;
    uint8_t txID;

    // Toggle LED0 to indicate that something was received (may not be good data).
    BSP_LED0_TOGGLE();

    // If any data has been received, check the IDs and signal if matching.
    if(RADIO_RECEIVE(rxBuf, &nwkID, &txID))
        {
            if((RADIO_NWK_ID == nwkID) && (USE_TX_ID == txID))
                {
            		// Toggle LED1 to indicate that device/nwk address matches
            		BSP_LED1_TOGGLE();

            		// Send payload via UART
            		msgLen = HAL_UART_FORMATTER(msgBuf, rxBuf, RADIO_PAY_LEN);
                    HAL_UART_TX(msgBuf,msgLen);
                }
        }

    calibrateAndRestartRX(&calibrateSem);

}

/**
 * Recalibrates the radio periodically
 */
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void) {

		calibrateAndRestartRX(&calibrateSem);
		IFG1 &= ~WDTIFG;
}

/**
 * Runs a calibration routine for the radio receiver, which needs to be
 * periodically calibrated to maintain good sensitivity and proper operation.
 */
void calibrateAndRestartRX(uint8_t* inUse) {

	if(*inUse) {
		return;
	}

	*inUse = 1;

    // Disable receive for a moment and recalibrate the radio
    RADIO_IDLE();

    // Calibrate the receiver
    RADIO_CALIBRATE();

    // Re-enable polling
    RADIO_RX_POLL();

	*inUse = 0;
}


/**
 * Analyzes the two char arrays given and returns the number of bits which
 *  vary between the two arrays. Good for computing BER and such.
 *
 * @param str1	First comparison string (char array)
 * @param str2	Second comparison string (char array)
 * @param len	The number of bytes to compare
 * @return	The number of bits which are different btwn the two strings
 */
uint16_t badBitCount(uint8_t* str1, uint8_t* str2, uint16_t len)
{
	uint16_t cnt = 0;	// Bad bit count
	uint8_t c1, c2;		// Temporary char storage for comparisons


	int i, j;

	for(i=0;i<len;i++){
		c1 = str1[i];
		c2 = str2[i];

		c1 = c1^c2;
		if(c1)
		{
			for(j=0;j<8;j++){
				if(c1 & 0x01u){
					cnt++;
				}

				c1>>=1;
			}
		}
	}
	return cnt;
}
