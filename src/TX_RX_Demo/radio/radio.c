/**
 * @brief Radio control functions for the CCxxxx IC series from TI.
 *
 * Low-power radio communications functions for use with CC2500, 1101, etc
 * from Texas Instruments (via Chipcon). Designed for minimum network overhead
 * and minimal energy usage wherever possible.
 *
 * @file radio.c
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0

 */

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "radio.h"			// Config data and referenced functions
#include "radio_register_map.h" // Radio settings mapped from SmartRF config file

///////////////////////////////////////////////////////////////////////////////
// Radio state variables
///////////////////////////////////////////////////////////////////////////////
// Buffer for parsing received data
uint8_t RADIO_rxBuf[RADIO_PKT_LEN];	// Receive buffer
uint8_t RADIO_txBuf[RADIO_PKT_LEN];	// Transmit buffer

void (*RADIO_rxCallback) (void);		// Callback pointer

enum RADIO_state_e // Tracks expected current state of radio
{
    // Only states which persist between RADIO_ function calls are needed here.
    RADIO_STATE_SLEEP,
    RADIO_STATE_IDLE,
    RADIO_STATE_RECEIVE_POLL
} RADIO_state;

uint8_t RADIO_rxNewData;

///////////////////////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////////////////////
// Compute delay to use after ~CS low edge based on current radio state
#define RADIO_CS_DLY()	((RADIO_state==RADIO_STATE_SLEEP) ? 5 : 0)

///////////////////////////////////////////////////////////////////////////////

/**
 * Initialize the radio with default settings.
 * Reset the radio and send all configuration data.
 *
 * @return RADIO_SUCCESS if everything worked properly, RADIO_FAIL if not.
 *
 * @pre	The radio has not been used since device power-on/reset
 * @todo 	Test this function
 */
int16_t RADIO_INIT( void )
{
    // Radio is in IDLE state by default upon power-on. Assume it's there now.
    RADIO_state = RADIO_STATE_IDLE;

    // SCLK = 1, SI = 0 to avoid potential problems with pin control mode
    BSP_SPI_POUT |= BSP_SPI_CLK_BIT | BSP_SPI_CS_BIT;
    BSP_SPI_POUT &= ~BSP_SPI_SIMO_BIT;

    // Hold CS high for >40us
    BSP_SPI_POUT &= ~ BSP_SPI_CS_BIT;
    HAL_DELAY_SHORT(45);
    BSP_SPI_POUT |= BSP_SPI_CS_BIT;

    // Wait for SO to go LOW after wakeup procedure
    while(BSP_SPI_PIN & BSP_SPI_SOMI_BIT);

    // Reset the radio. Assumes the radio has just powered on and is IDLE.
    HAL_SPI_STROBE(CC2500_SRES, 0); //@todo make a macro for strobe values (better portability)

    // Wait for reset to complete (hold on a HI SOMI line).
    while(BSP_SPI_PIN & BSP_SPI_SOMI_BIT);

    //Transmit all configuration register values as a block.
    //	It's faster to transmit all registers than to hop btwn necessary ones.
    HAL_SPI_WRITE((RADIO_REG_BLOCK_START | CC2500_WRITE_BURST), (uint8_t*)RADIO_REG_SETTINGS, RADIO_REG_BLOCK_LEN, 0);

    // @todo Make sure radio is idle at this point

    // Clear newData flag
    RADIO_rxNewData = 0;

    return RADIO_SUCCESS;
}

/**
 * Configure the receive callback system and start the radio receiver.
 *
 * @param rxCallback the function to call when something has been received.
 * @return RADIO_SUCCESS if everything worked properly, RADIO_FAIL if not.
 *
 * @todo Test this function
 */
int16_t RADIO_SETUP_RX( void (*rxCallback)(void) )
{
    // Store callback function in local var.
    RADIO_rxCallback = rxCallback;

    // Configure GDO line to produce an edge upon received data.
    //	Already configured in RADIO_INIT() register configuration.
    // @todo If CRC enabled, config GDO to produce edge only on good CRC.

    // Configure interrupting on proper GDO signal.
    BSP_GDO_PIES |= BSP_GDO0_BIT; // Interrupt on falling edge of Sync signal
    BSP_GDO_PIFG &= ~BSP_GDO0_BIT;
    BSP_GDO_PIE |= BSP_GDO0_BIT;

    // Place the radio in polling receive state
    RADIO_RX_POLL();

    // Flush the RX buffer
    //HAL_SPI_STROBE(CC2500_SFRX, 0);

    // Delay by > 88.4 us to transition from IDLE to RX
    HAL_PRECISE_DELAY(3); // 3*30.5us > 88.4us

    // Update local state variable
    RADIO_state = RADIO_STATE_RECEIVE_POLL;

    return RADIO_SUCCESS;
}

/**
 * Strobes the radio to start receive polling. Must have called RADIO_SETUP_RX
 * prior to using this function.
 */
int16_t RADIO_RX_POLL( void )
{
    HAL_SPI_STROBE(CC2500_SRX, RADIO_CS_DLY());

    return RADIO_SUCCESS;
}

/**
  * Parses the received data and copies the payload into the given user array.
  * Currently supports only statically sized payloads.
  *
  * @param dest A pointer to the first location of the destination array.
  * @param nwkID updated to the network ID used by the transmitter.
  * @param txID updated to the transmitter's device ID.
  * @return The size of the receive buffer in bytes.
  *
  * @todo Finish this function
  */
uint16_t RADIO_RECEIVE( uint8_t* dest, uint8_t* nwkID, uint8_t* txID)
{
    uint16_t i;

    // Enter critical so that data buffer won't be overwritten.
    HAL_ENTER_CRITICAL();

    // Check for new data prior to copying buffer.
    if(!RADIO_rxNewData)
        {
            return 0;
        }

    // Copy the received packet from local buffer to user's data buffer.
    for(i=0; i<RADIO_PAY_LEN; i++)
        {
            dest[i] = RADIO_rxBuf[i + RADIO_HDR_LEN];
        }

    // Mutate nwkID and txID appropriately according to received data.
    *nwkID = RADIO_rxBuf[0];
    *txID = RADIO_rxBuf[1];

    // Clear new data flag.
    RADIO_rxNewData = 0;

    // Exit critical, allowing for buffer to be written to.
    HAL_EXIT_CRITICAL();

    // @todo Handle RX timeout termination

    // Return length of payload received
    return RADIO_PAY_LEN;
}

/**
  * Set the transmit power of the radio
  *
  * @param pwr The indexed transmit power, as described in the CCxxxx datasheet.
  * @return RADIO_SUCCESS if everything worked properly, RADIO_FAIL if not.
  *
  * @todo Test this function
  */
int16_t RADIO_SET_TX_PWR( uint8_t pwr )
{
    // Write new setting to appropriate register in PA TABLE.
    HAL_SPI_WRITE((CC2500_PATABLE | CC2500_WRITE_SINGLE), &pwr, 1, RADIO_CS_DLY());

    // @todo Is this the correct state transition logic?

    // If we've woken up the radio, it will stay in Idle state.
    if(RADIO_STATE_SLEEP == RADIO_state)
        RADIO_state = RADIO_STATE_IDLE;

    return RADIO_SUCCESS;
}

/**
  * Broadcast the network ID and device ID, followed by the payload itself.
  * Packet format = {RADIO_NWK_ID, RADIO_DEV_ID, {Static Size Payload}}
  * Currently only supports statically sized payload.
  *
  * @param msg a pointer to the array containing the payload
  * @return RADIO_SUCCESS if everything worked properly, RADIO_FAIL if not.
  *
  * @todo Test this function
  */
int16_t RADIO_TX( uint8_t* msg )
{
    int i;

    // Copy header data into txBuf
    RADIO_txBuf[0] = RADIO_NWK_ID; 	// @todo Do these assignments during init (once only)
    RADIO_txBuf[1] = RADIO_DEV_ID;

    // Copy payload data into txBuf
    // @todo Do this without a string copy...
    for(i = RADIO_HDR_LEN; i < RADIO_PKT_LEN; i++ )
        {
            RADIO_txBuf[i] = msg[i-RADIO_HDR_LEN];
        }

    // Flush TX FIFO buffer
    // @todo Determine if this is needed
    HAL_SPI_STROBE(CC2500_SFTX, RADIO_CS_DLY());

    //@todo: Load all data from user buffer into the radio TX FIFO via SPI.
    //		Use SPI block write for this.
    HAL_SPI_WRITE((CC2500_TXFIFO | CC2500_WRITE_BURST), RADIO_txBuf, RADIO_PKT_LEN, 0);

    //@todo: Go into TX mode and transmit the data!

    HAL_SPI_STROBE(CC2500_STX, 0);

    //@todo Wait for transmission to complete??
    // Wait for GDO2 to go HI and LO again indicating TX has finished.
    // Only works when GDO2 IOCFG is set to 0x06.
    while(BSP_GDO_PIN & BSP_GDO2_BIT);

    //@todo: If CCA is enabled, watch for a CCA failure.
    //		Do NOT try again on CCA fail.

    // Update local state variable
    // Assuming radio is configured to IDLE after transmit is complete.
    RADIO_state = RADIO_STATE_IDLE;

    return RADIO_SUCCESS;
}

/*
 * Command the radio to perform manual frequency synth calibration routine.
 * Blocks until calibration is complete (~720 us for CC2500).
 *
 * @return RADIO_SUCCESS if everything worked properly, RADIO_FAIL if not.
 *
 * @todo Test this function
 */
int16_t RADIO_CALIBRATE( void )
{
    // Send command strobe
    HAL_SPI_STROBE(CC2500_SCAL, RADIO_CS_DLY());

    // @todo Delay for calibration time
    HAL_PRECISE_DELAY(24); // 30.5us*24 > 721 us (calibration time from datasheet).

    // Update local state variable
    RADIO_state = RADIO_STATE_IDLE;

    return RADIO_SUCCESS;
}

/**
 * Place the radio in Sleep state
 *
 * @return RADIO_SUCCESS if everything worked properly, RADIO_FAIL if not.
 *
 * @todo Test this function
 */
int16_t RADIO_SLEEP( void )
{

    // Transmit sleep strobe
    HAL_SPI_STROBE(CC2500_SPWD, RADIO_CS_DLY());
    //Disable SPI
    BSP_SPI_POUT 	&=	~BSP_SPI_CS_BIT;
    // Update local state variables
    RADIO_state = RADIO_STATE_SLEEP;

    return RADIO_SUCCESS;
}

/**
 * Place the radio in Idle state
 *
 * @return RADIO_SUCCESS if everything worked properly, RADIO_FAIL if not.
 *
 * @todo Test this function
 */
int16_t RADIO_IDLE( void )
{

    // Transmit idle strobe
    HAL_SPI_STROBE(CC2500_SIDLE, RADIO_CS_DLY());

    // Update local state variables
    RADIO_state = RADIO_STATE_IDLE;

    return RADIO_SUCCESS;
}

/**
 * Interrupt vector for GDO pins. Performs rxCallback when data received.
 *
 * @todo Rigorously test this function
 * @todo Don't allow through any packets which fail CRC!!
 */
#pragma vector=BSP_GDO_VECTOR
__interrupt void RADIO_GDO_ISR ( void )
{
	// Disable port interrupt to avoid nested interrupting
    BSP_GDO_PIE &= ~BSP_GDO0_BIT;

    // Copy the receive FIFO contents into the local rx Buffer
    HAL_SPI_READ(CC2500_RXFIFO | CC2500_READ_BURST, RADIO_rxBuf, RADIO_PKT_LEN, RADIO_CS_DLY());

    // Signal that new data is present
    RADIO_rxNewData = 1;

    // Enable interrupts to avoid confusion in rxCallback()
    HAL_ENABLE_INTERRUPTS();

    // Call the user's callback function in an ISR context.
    RADIO_rxCallback();

    // Turn off global interrupts for remainder of ISR to avoid nesting
    HAL_DISABLE_INTERRUPTS();

    // Re-enable receive interrupt
    BSP_GDO_PIE |= BSP_GDO0_BIT;

    // Clear IFG
    P2IFG = 0x00;
    // The radio is assumed to be configured to remain in RX state after receive.
}

///////////////////////////////////////////////////////////////////////////////
