/**
 * @brief Provides an interface for the radio control functions for the CCxxxx
 * 			IC series from TI.
 *
 * Low-power radio communications functions for use with CC2500, 1101, etc
 * from Texas Instruments (via Chipcon). Designed for minimum overhead
 * and minimal energy usage wherever possible.
 *
 * @file radio.h
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

/*---------------------Include Guard-----------------------------------------*/
#ifndef RADIO_H
#define RADIO_H
/*---------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
/// Packet sizing information
///////////////////////////////////////////////////////////////////////////////

// Radio packet header length
#define RADIO_HDR_LEN	2

// Transmitted/received packet size in bytes {HDR, PAYLOAD}
#define RADIO_PKT_LEN	RADIO_HDR_LEN + RADIO_PAY_LEN

// Number of low-power timer cycles for radio to wake from sleep mode after
//	CS line pulled low.
#define	RADIO_CS_DLY_TIME	5

///////////////////////////////////////////////////////////////////////////////
/// Return status definitions
///////////////////////////////////////////////////////////////////////////////
#define RADIO_SUCCESS 	0	// Successful completion of function
#define RADIO_FAIL		1	// Non-specific failure occurred
#define RADIO_CCA_FAIL 	2	// Clear channel assessment failed

///////////////////////////////////////////////////////////////////////////////
/// Includes
///////////////////////////////////////////////////////////////////////////////
#include "../config/config.h"// Platform-specific definitions
#include "../hal/hal.h" 	// Hardware abstraction layer functions
#include "../hal/bsp.h"		// Board-specific functions
#include <stdint.h>			// Data type definitions


///////////////////////////////////////////////////////////////////////////////
/// Prototypes
///////////////////////////////////////////////////////////////////////////////

// Initialize the radio with default settings, and leave it in Idle state.
int16_t RADIO_INIT( void );
// Configure an RX callback function
int16_t RADIO_SETUP_RX(void (*rxCallback)(void));
// Start polling
int16_t RADIO_RX_POLL( void );

// Set radio state
int16_t RADIO_SLEEP();
int16_t RADIO_IDLE();


// Turn on/off receive polling
int16_t RADIO_RX_ON();
int16_t RADIO_RX_OFF();

// Copy received data to dest array, and report network and transmitter ID
uint16_t RADIO_RECEIVE( uint8_t* dest, uint8_t* nwkID, uint8_t* txID);

// Set the transmit power level
int16_t RADIO_SET_TX_PWR(uint8_t pwr);

// Send a packet with the given payload message
int16_t RADIO_TX(uint8_t* msg );

// Command the radio to perform manual frequency synth calibration routine.
int16_t RADIO_CALIBRATE( void );


///////////////////////////////////////////////////////////////////////////////
#endif /* RADIO_H */
///////////////////////////////////////////////////////////////////////////////
