/**
 * @brief Platform-specific IO assignments and other info
 *
 *
 * @file config.h
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0
 */

/*---------------------Include Guard-----------------------------------------*/
#ifndef CONFIG_H
#define CONFIG_H
/*---------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
//	Platform-specific includes
///////////////////////////////////////////////////////////////////////////////
#include "config_warp_rev1.h"
#include "cc2500.h"
#include "smartrf_CC2500.h"

///////////////////////////////////////////////////////////////////////////////
// Configuration
///////////////////////////////////////////////////////////////////////////////

// Should debug features be built?
#define HAL_DEBUG 			FALSE

// Desired active-mode clock frequency in MHz. Options are: 1,4,8,16.
/// @todo Listen to this value in clock initialization routines
#define HAL_CLOCK_FREQ		1

// Should init routine optimizations be done (Removes auto-generated
//	initialization routine)?
#define HAL_OPTIMIZE_INIT	TRUE

// Network config information
#define RADIO_NWK_ID		0x88// The ID for this network
#define RADIO_DEV_ID		0x77// This device address

// Transmit packet payload length
#define RADIO_PAY_LEN		6	// {{Sensor 1 ID}, {MSB1}, {LSB1}, {Sensor 2 ID}, {MSB2}, {LSB2}}

// Essential transmit/receive settings
#define RADIO_USE_FEC		TRUE
#define RADIO_USE_CRC		FALSE

/// @todo Listen to the following config values...
#define RADIO_TX_CCA 		FALSE	// CCA on or off


///////////////////////////////////////////////////////////////////////////////
#endif /* CONFIG_H */
///////////////////////////////////////////////////////////////////////////////
