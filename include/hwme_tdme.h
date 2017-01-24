/**
 * @file hwme_tdme.h
 * @brief Definitions for Cascoda's HWME and TDME.
 *//*
 * Copyright (C) 2016  Cascoda, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef HWME_TDME_H
#define HWME_TDME_H

#define MAX_HWME_ATTRIBUTE_SIZE  16
#define MAX_TDME_ATTRIBUTE_SIZE  2


/******************************************************************************/
/****** HWME Enumerations                                                ******/
/******************************************************************************/
/* Status */
#define HWME_SUCCESS           (0x00)
#define HWME_UNKNOWN           (0x01)
#define HWME_INVALID           (0x02)
#define HWME_NO_ACCESS         (0x03)
#define HWME_MIN_STATUS         HWME_SUCCESS
#define HWME_MAX_STATUS         HWME_NO_ACCESS
/* WakeUpCondition */
/** Transceiver woken up from Power Up / System Reset */
#define HWME_WAKEUP_POWERUP    (0x00)
/** Watchdog Timer Time-Out */
#define HWME_WAKEUP_WDT        (0x01)
/** Transceiver woken up from Power-Off by Sleep Timer Time-Out */
#define HWME_WAKEUP_POFF_SLT   (0x02)
/** Transceiver woken up from Power-Off by GPIO Activity */
#define HWME_WAKEUP_POFF_GPIO  (0x03)
/** Transceiver woken up from Standby by Sleep Timer Time-Out */
#define HWME_WAKEUP_STBY_SLT   (0x04)
/** Transceiver woken up from Standby by GPIO Activity */
#define HWME_WAKEUP_STBY_GPIO  (0x05)
/** Sleep-Timer Time-Out in Active Mode */
#define HWME_WAKEUP_ACTIVE_SLT (0x06)
#define HWME_MIN_WAKEUP         HWME_WAKEUP_POWERUP
#define HWME_MAX_WAKEUP         HWME_WAKEUP_ACTIVE_SLT
/* LqiMode */
#define HWME_LQIMODE_CS        (0x00) //!< CS (Carrier Sense) is reported as LQI
#define HWME_LQIMODE_ED        (0x01) //!< ED (Energy Detect) is reported as LQI


/******************************************************************************/
/****** HWME Attribute IDs                                               ******/
/******************************************************************************/
#define HWME_POWERCON          (0x00) //!< Power Saving Mode Control
#define HWME_CHIPID            (0x01) //!< Product ID and Version Number
#define HWME_TXPOWER           (0x02) //!< Transmit Power Setting
/** Clear Channel Assessment Mode, according to 802.15.4 Section 6.9.9 */
#define HWME_CCAMODE           (0x03)
#define HWME_EDTHRESHOLD       (0x04) //!< Energy Detect (ED) Threshold for CCA
#define HWME_CSTHRESHOLD       (0x05) //!< Carrier Sense (CS) Threshold for CCA
/** Energy Detect (ED) Value of current channel */
#define HWME_EDVALUE           (0x06)
/** Carrier Sense (CS) Value of current channel */
#define HWME_CSVALUE           (0x07)
/** Energy Detect (ED) Value of last received Packet */
#define HWME_EDVALLP           (0x08)
/** Carrier Sense (CS) Value of last received Packet */
#define HWME_CSVALLP           (0x09)
/** Frequency Offset of last received Packet */
#define HWME_FREQOFFS          (0x0A)
#define HWME_MACTIMER          (0x0B) //!< MAC Symbol Timer Value
#define HWME_RANDOMNUM         (0x0C) //!< Random Number Value
#define HWME_TEMPERATURE       (0x0D) //!< Temperature Sensor Measurement Value
/** Host Security Key for AES-128 Encryption or Decryption */
#define HWME_HSKEY             (0x0E)
/** System Clock Output to a specific GPIO */
#define HWME_SYSCLKOUT         (0x0F)
/** LQI Reporting Mode for Data Indications and PanDescriptors */
#define HWME_LQIMODE           (0x10)
#define HWME_MIN_ATTRIBUTE     HWME_POWERCON
#define HWME_MAX_ATTRIBUTE     HWME_LQIMODE


/******************************************************************************/
/****** TDME Enumerations                                                ******/
/******************************************************************************/
/* Hardware Error Messages from TDME_ERROR_indication */
#define TDME_ERR_NO_ERROR     (0x00)  //!< Everything OK
#define TDME_ERR_LO_UNLOCK    (0x10)  //!< LO Unlocked
#define TDME_ERR_SPI_INVALID  (0x20)  //!< Invalid SPI Message
#define TDME_ERR_EX_STOF      (0x30)  //!< Exception: Stack Overflow
#define TDME_ERR_EX_STUF      (0x40)  //!< Exception: Stack Underflow
#define TDME_ERR_EX_DIVBY0    (0x50)  //!< Exception: Divide by 0
#define TDME_ERR_DMA_ACCESS   (0x60)  //!< DMA Access Overflow/Underflow (+0x0F)
#define TDME_ERR_UNEXP_IRQ    (0x70)  //!< Unexpected Interrupt (+0x0F)
#define TDME_ERR_MALLOC_FAIL  (0x80)  //!< Memory Allocation Failure
#define TDME_ERR_SYSTEM_ERROR (0x90)  //!< System Error
/* Status */
#define TDME_SUCCESS          (0x00)
#define TDME_UNKNOWN          (0x01)
#define TDME_INVALID          (0x02)
#define TDME_NO_ACCESS        (0x03)
#define TDME_LO_ERROR         (0x04)
#define TDME_FCS_ERROR        (0x05)
#define TDME_SHR_ERROR        (0x06)
#define TDME_PHR_ERROR        (0x07)
#define TDME_MIN_STATUS       TDME_SUCCESS
#define TDME_MAX_STATUS       TDME_PHR_ERROR
/* Test Modes */
#define TDME_TEST_OFF         (0x00) //!< All Test Modes are disabled (default)
/** Idle Test Mode. Test mode is enabled but transmitter and receiver are off */
#define TDME_TEST_IDLE        (0x01)
#define TDME_TEST_TX          (0x02) //!< Transmit Test Mode
#define TDME_TEST_RX          (0x03) //!< Receive Test Mode
/** Energy Detect Test Mode (Energy Sniffer) */
#define TDME_TEST_ED          (0x04)
/** LO Test Mode 1 (Tx/Rx with no Modulation on 802.15.4 Channel, PA enabled) */
#define TDME_TEST_LO_1        (0x05)
/** LO Test Mode 2 (VCO Open Loop / Initialisation, PA enabled) */
#define TDME_TEST_LO_2        (0x06)
/** LO Test Mode 3 (Locking Test, PA disabled) */
#define TDME_TEST_LO_3        (0x07)
#define TDME_MIN_TESTMODE     TDME_TEST_OFF
#define TDME_MAX_TESTMODE     TDME_TEST_LO_3
/* Tx Packet PHY Payload Data Type */
#define TDME_TXD_RANDOM       (0x00) //!< Random Data, internally generated
/** Sequence Number in 1st Byte and all other Data Random, internally
 *  generated */
#define TDME_TXD_SEQRANDOM    (0x01)
/** Count (Increment) Data, internally generated */
#define TDME_TXD_COUNT        (0x02)
/** Data external and appended to TDME-TXPKT Request */
#define TDME_TXD_APPENDED     (0x03)
#define TDME_MIN_TXD          TDME_TXD_RANDOM
#define TDME_MAX_TXD          TDME_TXD_APPENDED


/******************************************************************************/
/****** TDME Attribute IDs                                               ******/
/******************************************************************************/
#define TDME_CHANNEL          (0x00) //!< IEEE802.15.4 Channel Selection
#define TDME_TX_CONFIG        (0x01) //!< Transmit (Tx) Test Configuration
#define TDME_ED_CONFIG        (0x02) //!< Energy Detect Configuration
#define TDME_RX_CONFIG        (0x03) //!< Receiver Test Configuration
/** LO Test 1 Configuration (Tx/Rx with no Modulation on 802.15.4 Channel) */
#define TDME_LO_1_CONFIG      (0x04)
/** LO Test 2 Configuration (VCO Open Loop / Initialisation) */
#define TDME_LO_2_CONFIG      (0x05)
#define TDME_ATM_CONFIG       (0x06) //!< Analog Test Bus Configuration
#define TDME_MPW2_OVWR        (0x07)
#define TDME_MIN_ATTRIBUTE    TDME_CHANNEL
#define TDME_MAX_ATTRIBUTE    TDME_MPW2_OVWR


/******************************************************************************/
/****** HWME/TDME/MAC Additional Enumerations                            ******/
/******************************************************************************/
/* CCA Mode */
#define CCAM_EDORCS     (0)   //!< CCA Mode: Either ED or CS level exceeded
#define CCAM_ED         (1)   //!< CCA Mode: ED level exceeded
#define CCAM_CS         (2)   //!< CCA Mode: CS level exceeded
#define CCAM_EDANDCS    (3)   //!< CCA Mode: Both ED and CS level exceeded


#endif // HWME_TDME_H
