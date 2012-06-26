/**
 * @brief Provides a const array based on radio register settings.
 *
 * Makes it easy to copy the settings over to the chip via SPI.
 * This file is specific to the CC2500. Actual register settings can
 * be found in the smartrf_CCXXXX header file.
 *
 * @file radio_register_map.h
 * @author Aaron Parks, UW Sensor Systems Laboratory
 * @version 1.0
 */

/*---------------------Include Guard-----------------------------------------*/
#ifndef RADIO_REGISTER_MAP_H
#define RADIO_REGISTER_MAP_H
/*---------------------------------------------------------------------------*/

#include "../config/config.h"
#include "radio.h"

// @todo Listen to #defines regarding CRC, CCA, etc.
// @todo Play with manual/auto calibration settings

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// The starting address of the critical register block in memory
#define RADIO_REG_BLOCK_START 0x00

// The number (starting from RADIO_REG_BLOCK_START) of registers which are critical.
#define RADIO_REG_BLOCK_LEN	0x2F // 0x00 to 0x2E

///////////////////////////////////////////////////////////////////////////////
//	Radio register settings array - Only necessary registers included
///////////////////////////////////////////////////////////////////////////////

const uint8_t RADIO_REG_SETTINGS[] =
{
    0x06,/*SMARTF_SETTING_IOCFG2*/ // MODIFIED to assert on TX and deassert on TXFIFO underflow
    0x29,/*SMARTRF_SETTING_IOCFG1*/// MODIFIED to mirror CHIP_RDYn
    0x06,/*SMARTRF_SETTING_IOCFG0*/ // MODIFIED from 0x0C to 0x06 (Sync) to cause new packet to assert GDO0
    SMARTRF_SETTING_FIFOTHR,
    SMARTRF_SETTING_SYNC1,
    SMARTRF_SETTING_SYNC0,
    RADIO_PKT_LEN,/*SMARTRF_SETTING_PKTLEN,*/

    //@todo Experiment with Preamble Quality Estimator Threshold (PQT), and try reading RSSI and CRC params.
    0x00, /*SMARTRF_SETTING_PKTCTRL1*/ // MODIFIED from 0x04 to 0x00 to disable status transmission

    /*SMARTRF_SETTING_PKTCTRL0*/ // MODIFIED from 0x12 to 0x00 to use fixed pkt len, and to use FIFOs. 0x04 to use CRC.
#if(RADIO_USE_CRC)
    0x04,
#else
    0x00,
#endif

    SMARTRF_SETTING_ADDR,
    SMARTRF_SETTING_CHANNR,
    SMARTRF_SETTING_FSCTRL1,
    SMARTRF_SETTING_FSCTRL0,
    SMARTRF_SETTING_FREQ2,
    SMARTRF_SETTING_FREQ1,
    SMARTRF_SETTING_FREQ0,
    SMARTRF_SETTING_MDMCFG4,
    SMARTRF_SETTING_MDMCFG3,
    SMARTRF_SETTING_MDMCFG2,//@todo Experiment with SYNC_MODE threshold

    /*SMARTRF_SETTING_MDMCFG1*/
#if(RADIO_USE_FEC)
    0x80 | SMARTRF_SETTING_MDMCFG1, // OR the existing setting with 0x80 to enable FEC.
#else
    SMARTRF_SETTING_MDMCFG1,
#endif

    SMARTRF_SETTING_MDMCFG0,
    SMARTRF_SETTING_DEVIATN,
    SMARTRF_SETTING_MCSM2, //@todo Learn more about RX_TIME and RX_TIME_QUAL
    0x3C, /*SMARTRF_SETTING_MCSM1*/ // MODIFIED from 0x30 to 0x3C to force radio to stay in RX after packet received
    0x08, /*SMARTRF_SETTING_MCSM0*/ // MODIFIED from 0x18 to 0x08 to disable autocal
    SMARTRF_SETTING_FOCCFG,
    SMARTRF_SETTING_BSCFG,
    SMARTRF_SETTING_AGCCTRL2, // @todo  Experiment with maximum gain settings
    SMARTRF_SETTING_AGCCTRL1,
    SMARTRF_SETTING_AGCCTRL0,
    SMARTRF_SETTING_WOREVT1,
    SMARTRF_SETTING_WOREVT0,
    SMARTRF_SETTING_WORCTRL,
    SMARTRF_SETTING_FREND1,
    SMARTRF_SETTING_FREND0,
    SMARTRF_SETTING_FSCAL3,
    SMARTRF_SETTING_FSCAL2,
    SMARTRF_SETTING_FSCAL1,
    SMARTRF_SETTING_FSCAL0,
    SMARTRF_SETTING_RCCTRL1,
    SMARTRF_SETTING_RCCTRL0,
    SMARTRF_SETTING_FSTEST,
    SMARTRF_SETTING_PTEST,
    SMARTRF_SETTING_AGCTEST,
    SMARTRF_SETTING_TEST2,
    SMARTRF_SETTING_TEST1,
    SMARTRF_SETTING_TEST0, // 0x2E, end of test section
    /*SMARTRF_SETTING_PARTNUM,
    SMARTRF_SETTING_VERSION,
    SMARTRF_SETTING_FREQEST,
    SMARTRF_SETTING_LQI,
    SMARTRF_SETTING_RSSI,
    SMARTRF_SETTING_MARCSTATE,
    SMARTRF_SETTING_WORTIME1,
    SMARTRF_SETTING_WORTIME0,
    SMARTRF_SETTING_PKTSTATUS,
    SMARTRF_SETTING_VCO_VC_DAC,
    SMARTRF_SETTING_TXBYTES,
    SMARTRF_SETTING_RXBYTES,
    SMARTRF_SETTING_RCCTRL1_STATUS,
    SMARTRF_SETTING_RCCTRL0_STATUS,*/
};

///////////////////////////////////////////////////////////////////////////////
#endif /* RADIO_REGISTER_MAP_H */
///////////////////////////////////////////////////////////////////////////////
