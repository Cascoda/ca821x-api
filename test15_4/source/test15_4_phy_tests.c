/**
 * @file test15_4_phy_tests.c
 * @brief PHY Test Functions
 * @author Wolfgang Bruchner
 * @date 19/07/14
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
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ca821x_api.h"

#include "test15_4_phy_tests.h"


/******************************************************************************/
/****** Global Variables used commonly                                   ******/
/******************************************************************************/
uint8_t                 PHY_TESTMODE;
uint8_t                 PHY_LASTTESTMODE;
uint8_t                 PHY_TESTSEQUENCECOUNT;
uint8_t                 PHY_TEST_REINIT_TDME_ATTRIBUTE;
struct PHYTestPar       PHY_TESTPAR;
struct PHYTestRes       PHY_TESTRES;


/******************************************************************************/
/****** Global Variables only used in this File                          ******/
/******************************************************************************/
uint16_t                PHY_TEST_COUNTTMP1;
uint16_t                PHY_TEST_COUNTTMP2;
uint16_t                PHY_TEST_COUNTTMP3;
uint8_t                 PHY_TEST_FLAG;

uint8_t                 PHYBuffer[256];
uint8_t                 PHYLength;
static long             start_ms; //!< Used for scheduling packet tx/rx

/******************************************************************************/
/***************************************************************************//**
 * \brief Returns how many milliseconds have elapsed since start_ms
 *******************************************************************************
 * \return milliseconds since start_ms
 *******************************************************************************
 ******************************************************************************/
static unsigned long calculate_elapsed_ms(void)
{
	unsigned long current_ms;

	current_ms = test15_4_getms();
	return current_ms - start_ms;
}


/******************************************************************************/
/***************************************************************************//**
 * \brief Dispatch Branch for PHY Tests including Initialisation
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestModeHandler(struct ca821x_dev *pDeviceRef)
{
	uint8_t status;

	if (PHY_TESTMODE & PHY_TEST_INIT) {
		if (((PHY_LASTTESTMODE & PHY_TEST_CMD) == PHY_TEST_RX_PSN) &&
		     ((PHY_TESTMODE     & PHY_TEST_CMD) != PHY_TEST_RX_PSN)) { /* Rx packet analysis at end of test */
			PHYTestStatistics2();
			PHYTestReportReceivedPacketAnalysis(pDeviceRef);
		}
		if (((PHY_LASTTESTMODE & PHY_TEST_CMD) == PHY_TEST_TX_PKT) &&
		     ((PHY_TESTMODE     & PHY_TEST_CMD) != PHY_TEST_TX_PKT)) { /* Tx packet analysis at end of test */
			PHYTestReportTransmitPacketAnalysis(pDeviceRef);
		}
		if ((status = PHYTestInitialise(pDeviceRef))) {
			printf(
				"ERROR INITIALISING PHY TEST! PHY_TESTSEQUENCECOUNT = %02X\n",
				PHY_TESTSEQUENCECOUNT
			);
			PHYTestExit(status, pDeviceRef);
		}
	} else if (PHY_TESTMODE & PHY_TEST_REINIT) {
		if ((status = PHYTestReinitialise(pDeviceRef))) {
			printf("ERROR REINITIALISING PHY TEST!\n");
			PHYTestExit(status, pDeviceRef);
		}
	} else if (PHY_TESTMODE == PHY_TEST_OFF) {
		printf("PHY_TESTMODE set to PHY_TEST_OFF.\n");
		PHYTestExit(MAC_SUCCESS, pDeviceRef);
	} else if (PHY_TESTMODE == PHY_TEST_TX_PKT) {
		if ((status = PHYTestTransmitPacket(pDeviceRef))) {
			printf("PHYTestTransmitPacket returned non-zero.\n");
			PHYTestExit(status, pDeviceRef);
		}
	} else if (PHY_TESTMODE == PHY_TEST_RX_PER) {
		PHYTestReceivePacketPER(pDeviceRef);
	} else if (PHY_TESTMODE == PHY_TEST_RX_PSN) {
		PHYTestReceivePacketPSN(pDeviceRef);
	} else if (PHY_TESTMODE == PHY_TEST_RX_EDSN) {
		PHYTestReceiveED(pDeviceRef);
	} else if (PHY_TESTMODE == PHY_TEST_LO_3) {
		if ((status = PHYTestLOLocking(pDeviceRef))) {
			PHYTestExit(status, pDeviceRef);
		}
	}

	/* Note: PHY_TEST_TX_CONT, PHY_TEST_LO_1 and PHY_TEST_LO_2 don't need
	 *       any interaction, only setup */
	 PHY_LASTTESTMODE = PHY_TESTMODE;
} // End of PHYTestModeHandler()


/******************************************************************************/
/***************************************************************************//**
 * \brief Sequential Initialisation of TDME
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 * The following steps are sequentially performed:\n
 *  0: Initialise: TDME_SET_request_sync TDME_CHANNEL\n
 *  1: Initialise: TDME_SET_request_sync TDME_TX_CONFIG\n
 *  2: Initialise: TDME_SET_request_sync TDME_ED_CONFIG\n
 *  3: Initialise: TDME_SET_request_sync TDME_RX_CONFIG\n
 *  4: Initialise: TDME_SET_request_sync TDME_LO_1_CONFIG\n
 *  5: Initialise: TDME_SET_request_sync TDME_LO_2_CONFIG\n
 *  6: Initialise: TDME_SET_request_sync TDME_ATM_CONFIG\n
 *  7: Initialise: TDME_SET_request_sync TDME_MPW2_OVWR\n
 *  8: Set Test Mode with TDME_TESTMODE_request_sync\n
 *  9: Finish Initialisation and start Test
 *******************************************************************************
 ******************************************************************************/
uint8_t PHYTestInitialise(struct ca821x_dev *pDeviceRef)
{
	uint8_t status;

	if(PHY_TESTSEQUENCECOUNT ==  0) {
		status = PHY_SET_request(TDME_CHANNEL, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else if (PHY_TESTSEQUENCECOUNT ==  1) {
		switch (PHY_TESTMODE & PHY_TEST_CMD) { /* set TXCONT according to test mode before TDME_SET_request */
		case PHY_TEST_TX_CONT:
		case PHY_TEST_LO_1:
		case PHY_TEST_LO_2:
			PHY_TESTPAR.TXCONT = 1;
			break;
		default:
			PHY_TESTPAR.TXCONT = 0;
			break;
		}
		status = PHY_SET_request(TDME_TX_CONFIG, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else if (PHY_TESTSEQUENCECOUNT ==  2) {
		status = PHY_SET_request(TDME_ED_CONFIG, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else if (PHY_TESTSEQUENCECOUNT ==  3) {
		status = PHY_SET_request(TDME_RX_CONFIG, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else if (PHY_TESTSEQUENCECOUNT ==  4) {
		status = PHY_SET_request(TDME_LO_1_CONFIG, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else if (PHY_TESTSEQUENCECOUNT ==  5) {
		status = PHY_SET_request(TDME_LO_2_CONFIG, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else if (PHY_TESTSEQUENCECOUNT ==  6) {
		status = PHY_SET_request(TDME_ATM_CONFIG, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else if(PHY_TESTSEQUENCECOUNT ==  7) {
		status = PHY_TESTMODE_request(pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else {
		status = MAC_SUCCESS;
		PHY_TESTMODE &= ~PHY_TEST_INIT;
		PHY_TESTSEQUENCECOUNT = 0;
		PHYTestInitTestResults();
		if(((PHY_TESTMODE & PHY_TEST_CMD) == PHY_TEST_TX_PKT) && PHY_TESTPAR.MACENABLED)
			status = PHYTestMACTxInitialise(pDeviceRef);
		if((((PHY_TESTMODE & PHY_TEST_CMD) == PHY_TEST_RX_PSN) ||
		    ((PHY_TESTMODE & PHY_TEST_CMD) == PHY_TEST_RX_PER)) &&
		   PHY_TESTPAR.MACENABLED)
			status = PHYTestMACRxInitialise(pDeviceRef);
		PHYTestReportTestMode(pDeviceRef);
		if((PHY_TESTMODE == PHY_TEST_TX_PKT) ||
		   (PHY_TESTMODE == PHY_TEST_RX_PSN) ||
		   (PHY_TESTMODE == PHY_TEST_LO_3))
			start_ms = test15_4_getms();
		if((PHY_TESTMODE & PHY_TEST_CMD) == PHY_TEST_LO_3) {
			PHY_TESTRES.TEST_RUNNING = 1;
			PHY_TEST_COUNTTMP1 =  0; // nr of locking tests
			PHY_TEST_COUNTTMP2 = 11; // channel
			PHY_TEST_COUNTTMP3 =  0; // rx_txb
		}
	}
	return status;
} // End of PHYTestInitialise()


/******************************************************************************/
/***************************************************************************//**
 * \brief Test Parameter Change and TDME Update
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 * Reinitialises specific TDME Attribute by TDME-SET if the PHY\n
 * EVBME PIB has been changed by EVBME-PHY_SET
 *
 * The following steps are sequentially performed:\n
 *  0: Initialise: TDME_SET_request_sync attribute\n
 *  1: Finish reinitialisation
 *******************************************************************************
 ******************************************************************************/
uint8_t PHYTestReinitialise(struct ca821x_dev *pDeviceRef)
{
	uint8_t status;

	if (PHY_TESTSEQUENCECOUNT == 0) {
		status = PHY_SET_request(PHY_TEST_REINIT_TDME_ATTRIBUTE, pDeviceRef);
		++PHY_TESTSEQUENCECOUNT;
	} else {
		PHY_TESTMODE &= ~PHY_TEST_REINIT;
		PHY_TESTSEQUENCECOUNT = 0;
		status = MAC_SUCCESS;
	}

	return status;
} // End of PHYTestReinitialise()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Transmit Packet
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 * The following steps are sequentially performed:\n
 *  0: Wait for Timeout and call TDME_TXPKT_request_sync()\n
 *  1: Report Transmission
 *******************************************************************************
 ******************************************************************************/
uint8_t PHYTestTransmitPacket(struct ca821x_dev *pDeviceRef)
{
	uint8_t status = 0;
	long msElapsed;

	if ((PHY_TESTMODE == PHY_TEST_TX_PKT) && (PHY_TESTRES.PACKET_COUNT == 0))
		PHY_TESTRES.TEST_RUNNING = 1;

	if (!PHY_TESTRES.TEST_RUNNING) {
		return status;
	}

	msElapsed = calculate_elapsed_ms();
	if (msElapsed <= PHY_TESTPAR.PACKETPERIOD) {
		return status;
	}

	start_ms = test15_4_getms();
	if (PHY_TESTPAR.MACENABLED) {
		status = PHY_TXPKT_MAC_request(pDeviceRef);
	} else {
		status = PHY_TXPKT_request(pDeviceRef);
	}

	if (status == TDME_LO_ERROR)
		status = MAC_SUCCESS;

	++PHY_TESTRES.PACKET_COUNT;
	++PHY_TEST_COUNTTMP1;

	/* report detailed packet information if packet period 500 ms or
	 * more */
	if ((PHY_TESTPAR.PACKETPERIOD >= 500) ||
	    ((PHY_TESTPAR.NUMBEROFPKTS <= 100) &&
	     (PHY_TESTPAR.NUMBEROFPKTS != 0))) {
		PHYTestReportPacketTransmitted(pDeviceRef);
	} else { /* report statistics every 5 to 12 seconds */
		if (((PHY_TESTPAR.PACKETPERIOD >=  250) && (PHY_TEST_COUNTTMP1 >=  20)) ||
		    ((PHY_TESTPAR.PACKETPERIOD >=  100) && (PHY_TEST_COUNTTMP1 >=  50)) ||
		    ((PHY_TESTPAR.PACKETPERIOD >=   50) && (PHY_TEST_COUNTTMP1 >= 100)) ||
		    ((PHY_TESTPAR.PACKETPERIOD >=   25) && (PHY_TEST_COUNTTMP1 >= 200)) ||
		    ((PHY_TESTPAR.PACKETPERIOD  <   25) && (PHY_TEST_COUNTTMP1 >= 500))) {
			PHYTestReportTransmitPacketAnalysis(pDeviceRef);
			PHY_TEST_COUNTTMP1 = 0;
		}
	}
	// stop test if number of packets is 100 or less
	if((PHY_TESTRES.PACKET_COUNT >= PHY_TESTPAR.NUMBEROFPKTS) &&
	   (PHY_TESTPAR.NUMBEROFPKTS != 0)) {
		PHY_TESTRES.TEST_RUNNING = 0;
		PHYTestExit(status, pDeviceRef);
	}

	return(status);
} // End of PHYTestTransmitPacket()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Receive Packet in PER Mode (Packet Error Rate)
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReceivePacketPER(struct ca821x_dev *pDeviceRef)
{
	long msElapsed;
	uint8_t missed_packet;

	if (PHY_TESTRES.TEST_RUNNING)
		msElapsed = calculate_elapsed_ms();

	if ((((msElapsed > PHY_TESTPAR.PACKETPERIOD) &&  PHY_TEST_FLAG) ||   /* 1.0 * PACKETPERIOD when previous packet was missed */
	     ((msElapsed > (PHY_TESTPAR.PACKETPERIOD + (PHY_TESTPAR.PACKETPERIOD >> 1))) && ~PHY_TEST_FLAG)) &&  /* 1.5 * PACKETPERIOD when previous packet was received */
	    (PHY_TESTPAR.PACKETPERIOD > 0) && PHY_TESTRES.TEST_RUNNING) { /* missed packet */
		missed_packet = 1;
		// printf("packet timed out\n");
	} else {
		missed_packet = 0;
	}

	if (PHY_TESTRES.PACKET_RECEIVED || missed_packet) {
		if ((PHY_TESTMODE == PHY_TEST_RX_PER) && (PHY_TESTRES.PACKET_COUNT == 0))
			PHY_TESTRES.TEST_RUNNING = 1;

		if (PHY_TESTRES.TEST_RUNNING)
			start_ms = test15_4_getms();

		if (missed_packet) {
			++PHY_TESTRES.PREERR_COUNT;
			PHY_TEST_FLAG = 1; /* last packet missed */
			missed_packet = 0;
		}

		if (PHY_TESTRES.PACKET_RECEIVED) {
			PHYTestStatistics1();
			PHY_TEST_FLAG = 0; /* last packet received */
			if(PHY_TESTPAR.PACKETPERIOD >= 500)
			PHYTestReportPacketReceived(pDeviceRef);
		}

		if (PHY_TESTRES.PACKET_COUNT <= PHY_TESTPAR.NUMBEROFPKTS) {
			++PHY_TESTRES.PACKET_COUNT;
			++PHY_TEST_COUNTTMP1;
		}

		if (PHY_TESTRES.TEST_RUNNING &&
		    (PHY_TESTRES.PACKET_COUNT >= PHY_TESTPAR.NUMBEROFPKTS)) {
			TDME_TESTMODE_request_sync(TDME_TEST_OFF, pDeviceRef); /* disable receiver at end */
			PHYTestStatistics3();
			PHYTestReportPERTestResult(pDeviceRef);
			PHY_TESTRES.TEST_RUNNING = 0;
			PHYTestExit(0, pDeviceRef);
		} else {
			if (((PHY_TESTPAR.PACKETPERIOD >= 1000) && (PHY_TEST_COUNTTMP1 >=   5)) ||
			    ((PHY_TESTPAR.PACKETPERIOD >=  250) && (PHY_TEST_COUNTTMP1 >=  20)) ||
			    ((PHY_TESTPAR.PACKETPERIOD >=  100) && (PHY_TEST_COUNTTMP1 >=  50)) ||
			    ((PHY_TESTPAR.PACKETPERIOD >=   50) && (PHY_TEST_COUNTTMP1 >= 100)) ||
			    ((PHY_TESTPAR.PACKETPERIOD >=   25) && (PHY_TEST_COUNTTMP1 >= 200)) ||
			    ((PHY_TESTPAR.PACKETPERIOD  <   25) && (PHY_TEST_COUNTTMP1 >= 500))) {
				if (PHY_TESTRES.PACKET_RECEIVED) {
					PHYTestStatistics2();
				}
				PHYTestReportReceivedPacketAnalysis(pDeviceRef);
				if (PHY_TESTRES.PACKET_RECEIVED) {
					/* reset averages */
					PHY_TESTRES.FO_AVG = 0;
					PHY_TESTRES.ED_AVG = 0;
					PHY_TESTRES.CS_AVG = 0;
				}
				PHY_TEST_COUNTTMP1 = 0;
			}
		}
		PHY_TESTRES.PACKET_RECEIVED = 0;
	}
} // End of PHYTestReceivePacketPER()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Receive Packet in in PSN Mode (Packet Sniffer)
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReceivePacketPSN(struct ca821x_dev *pDeviceRef)
{
	int msElapsed;

	if (PHY_TESTRES.PACKET_RECEIVED) {
		PHY_TESTRES.PACKET_RECEIVED = 0;
		PHYTestStatistics1();
		++PHY_TESTRES.PACKET_COUNT;
		++PHY_TEST_COUNTTMP1;
		PHYTestReportPacketReceived(pDeviceRef);
	} else {
		msElapsed = calculate_elapsed_ms();
		if (msElapsed > PHY_TEST_REPORT_PERIOD) {
			start_ms = test15_4_getms();
			PHYTestStatistics2();
			//PHYTestReportReceivedPacketAnalysis();
			/* reset averages */
			PHY_TESTRES.FO_AVG = 0;
			PHY_TESTRES.ED_AVG = 0;
			PHY_TESTRES.CS_AVG = 0;
			PHY_TEST_COUNTTMP1 = 0;
		}
	}
} // End of PHYTestReceivePacketPSN()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Receive in ED Detect Mode (Energy Sniffer)
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReceiveED(struct ca821x_dev *pDeviceRef)
{
	if (PHY_TESTRES.PACKET_RECEIVED) {
		PHY_TESTRES.PACKET_RECEIVED = 0;
		++PHY_TESTRES.PACKET_COUNT;
		PHYTestReportEDReceived(pDeviceRef);
	}
} // End of PHYTestReceiveED()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test LO_3 (Locking) Test
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 * The following steps are sequentially performed:\n
 * for each CHANNEL and LO_3_LOCKS:\n
 *  0: Wait for LO_3_PERIOD Timeout and call TDME_LOTLK_request()\n
 *  1: Report Test Result
 *
 * Parameter Storage:\n
 * PHY_TEST_COUNTTMP1:  Nr of Locking Tests\n
 * PHY_TEST_COUNTTMP2:  Channel\n
 * PHY_TEST_COUNTTMP3:  Rx/Txb
 *******************************************************************************
 ******************************************************************************/
uint8_t PHYTestLOLocking(struct ca821x_dev *pDeviceRef)
{
	uint8_t status;
	uint8_t ntest, channel, rx_txb;
	int msElapsed;

	if (!PHY_TESTRES.TEST_RUNNING) {
		PHY_TESTSEQUENCECOUNT = 0;
		return MAC_SUCCESS;
	}

	ntest   = (uint8_t)PHY_TEST_COUNTTMP1;
	channel = (uint8_t)PHY_TEST_COUNTTMP2;
	rx_txb  = (uint8_t)PHY_TEST_COUNTTMP3;
	msElapsed = calculate_elapsed_ms();
	if (msElapsed > PHY_TESTPAR.LO_3_PERIOD) {
		start_ms = test15_4_getms();
		PHY_TESTSEQUENCECOUNT = 1;
		status = PHY_LOTLK_request(channel, rx_txb, pDeviceRef);
		if(status == TDME_LO_ERROR) {
			status = MAC_SUCCESS;
		}
	} else {
		if (PHY_TESTSEQUENCECOUNT ==  1) {
			PHY_TESTSEQUENCECOUNT = 0;
			PHYTestReportLOLocking(ntest, pDeviceRef);
			if (ntest < PHY_TESTPAR.LO_3_LOCKS-1) {
				++PHY_TEST_COUNTTMP1;
			} else {
				PHY_TEST_COUNTTMP1 = 0;
				if (channel < 26) {
					++PHY_TEST_COUNTTMP2;
				} else {
					PHY_TEST_COUNTTMP2 = 11;
					if (rx_txb == 0) {
						PHY_TEST_COUNTTMP3 = 1;
					} else {
						PHY_TEST_COUNTTMP1 = 0;
						PHY_TEST_COUNTTMP2 = 0;
						PHY_TEST_COUNTTMP3 = 0;
						PHY_TESTRES.TEST_RUNNING = 0;
					}
				}
			}
			status = MAC_SUCCESS;
		} else {
			PHY_TESTSEQUENCECOUNT = 0;
			status = MAC_SUCCESS;
		}
	}
	return(status);
} // End of PHYTestLOLocking()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Mode Exit
 *******************************************************************************
 * \param status - Status to be reported
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestExit(uint8_t status, struct ca821x_dev *pDeviceRef)
{
	PHY_TESTMODE = PHY_TEST_OFF;
	PHY_TESTSEQUENCECOUNT = 0;
	PHYTestReportExit(status, pDeviceRef);
} // End of PHYTestExit()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Initialise Results and Run Parameters
 *******************************************************************************
 ******************************************************************************/
void PHYTestInitTestResults(void)
{
	PHY_TESTRES.SEQUENCENUMBER  = 0;
	PHY_TESTRES.PACKET_COUNT    = 0;
	PHY_TESTRES.PACKET_RECEIVED = 0;
	PHY_TESTRES.TEST_RUNNING    = 0;
	PHY_TESTRES.LOERR_COUNT     = 0;
	PHY_TESTRES.CRCERR_COUNT    = 0;
	PHY_TESTRES.PHRERR_COUNT    = 0;
	PHY_TESTRES.SHRERR_COUNT    = 0;
	PHY_TESTRES.PREERR_COUNT    = 0;
	PHY_TESTRES.FO_AVG          = 0;
	PHY_TESTRES.ED_AVG          = 0;
	PHY_TESTRES.CS_AVG          = 0;
	PHY_TESTRES.FO_AVG_TOTAL    = 0;
	PHY_TESTRES.ED_AVG_TOTAL    = 0;
	PHY_TESTRES.CS_AVG_TOTAL    = 0;
	PHY_TESTRES.ED_MAX          = 0;
	PHY_TESTRES.ED_MIN          = 255;
	PHY_TESTRES.CS_MAX          = 0;
	PHY_TESTRES.CS_MIN          = 255;

	PHY_TEST_COUNTTMP1          = 0;
	PHY_TEST_COUNTTMP2          = 0;
	PHY_TEST_COUNTTMP3          = 0;
	PHY_TEST_FLAG               = 0;
} // End of PHYTestInitTestResults()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Reset Testmode PIB
 *******************************************************************************
 ******************************************************************************/
void PHYTestReset(void)
{
	PHY_TESTMODE           = PHY_TEST_OFF;
	PHY_TESTSEQUENCECOUNT  = 0;
	PHY_TESTPAR.MACENABLED = 0;

	PHY_TESTPAR.PACKETPERIOD    = PHY_TESTPARDEF_PACKETPERIOD;
	PHY_TESTPAR.PACKETLENGTH    = PHY_TESTPARDEF_PACKETLENGTH;
	PHY_TESTPAR.NUMBEROFPKTS    = PHY_TESTPARDEF_NUMBEROFPKTS;
	PHY_TESTPAR.PACKETDATATYPE  = PHY_TESTPARDEF_PACKETDATATYPE;
	PHY_TESTPAR.CHANNEL         = PHY_TESTPARDEF_CHANNEL;
	PHY_TESTPAR.TXPOWER_IB      = PHY_TESTPARDEF_TXPOWER_IB;
	PHY_TESTPAR.TXPOWER_PB      = PHY_TESTPARDEF_TXPOWER_PB;
	PHY_TESTPAR.TXPOWER_BOOST   = PHY_TESTPARDEF_TXPOWER_BOOST;
	PHY_TESTPAR.TXCONT          = PHY_TESTPARDEF_TXCONT;
	PHY_TESTPAR.EDTHRESHOLD     = PHY_TESTPARDEF_EDTHRESHOLD;
	PHY_TESTPAR.RX_FFSYNC       = PHY_TESTPARDEF_RX_FFSYNC;
	PHY_TESTPAR.LO_1_RXTXB      = PHY_TESTPARDEF_LO_1_RXTXB;
	PHY_TESTPAR.LO_2_FDAC       = PHY_TESTPARDEF_LO_2_FDAC;
	PHY_TESTPAR.LO_3_LOCKS      = PHY_TESTPARDEF_LO_3_LOCKS;
	PHY_TESTPAR.LO_3_PERIOD     = PHY_TESTPARDEF_LO_3_PERIOD;
	PHY_TESTPAR.ATM             = PHY_TESTPARDEF_ATM;
	PHY_TESTPAR.MPW2_OVWR       = PHY_TESTPARDEF_MPW2_OVWR;
} // End of PHYTestReset()


/******************************************************************************/
/***************************************************************************//**
 * \brief Calculate Test Statistics for each Packet
 *******************************************************************************
 ******************************************************************************/
void PHYTestStatistics1(void)
{
	int8_t sv;

	/* sums for averages */
	if (PHY_TEST_COUNTTMP1 < 256) {
		sv = (int8_t)PHYBuffer[3];
		PHY_TESTRES.FO_AVG += (int16_t)sv;
		PHY_TESTRES.ED_AVG += (uint16_t)PHYBuffer[1];
		PHY_TESTRES.CS_AVG += (uint16_t)PHYBuffer[2];
		++PHY_TEST_COUNTTMP2;
	}
	/* determine min/max */
	if(PHYBuffer[1] > PHY_TESTRES.ED_MAX)
		PHY_TESTRES.ED_MAX = PHYBuffer[1];
	if(PHYBuffer[1] < PHY_TESTRES.ED_MIN)
		PHY_TESTRES.ED_MIN = PHYBuffer[1];
	if(PHYBuffer[2] > PHY_TESTRES.CS_MAX)
		PHY_TESTRES.CS_MAX = PHYBuffer[2];
	if(PHYBuffer[2] < PHY_TESTRES.CS_MIN)
		PHY_TESTRES.CS_MIN = PHYBuffer[2];
} // End of PHYTestStatistics1()


/******************************************************************************/
/***************************************************************************//**
 * \brief Calculate Test Statistics before each Analysis
 *******************************************************************************
 ******************************************************************************/
void PHYTestStatistics2(void)
{
	if (PHY_TEST_COUNTTMP2 > 0) {
		/* determine averages */
		PHY_TESTRES.FO_AVG = divs16round(PHY_TESTRES.FO_AVG, (int16_t)PHY_TEST_COUNTTMP2);
		PHY_TESTRES.ED_AVG = divu16round(PHY_TESTRES.ED_AVG, (uint16_t)PHY_TEST_COUNTTMP2);
		PHY_TESTRES.CS_AVG = divu16round(PHY_TESTRES.CS_AVG, (uint16_t)PHY_TEST_COUNTTMP2);
		/* sums for overall averages */
		if (PHY_TEST_COUNTTMP3 < 256) {
			PHY_TESTRES.FO_AVG_TOTAL += PHY_TESTRES.FO_AVG;
			PHY_TESTRES.ED_AVG_TOTAL += PHY_TESTRES.ED_AVG;
			PHY_TESTRES.CS_AVG_TOTAL += PHY_TESTRES.CS_AVG;
			++PHY_TEST_COUNTTMP3;
		}
	}
	PHY_TEST_COUNTTMP2 = 0;
} // End of PHYTestStatistics2()


/******************************************************************************/
/***************************************************************************//**
 * \brief Calculate Test Statistics at End of Test
 *******************************************************************************
 ******************************************************************************/
void PHYTestStatistics3(void)
{
	/* determine total averages */
	if (PHY_TEST_COUNTTMP3 > 0) {
		PHY_TESTRES.FO_AVG_TOTAL = divs16round(PHY_TESTRES.FO_AVG_TOTAL, (int16_t)PHY_TEST_COUNTTMP3);
		PHY_TESTRES.ED_AVG_TOTAL = divu16round(PHY_TESTRES.ED_AVG_TOTAL, (uint16_t)PHY_TEST_COUNTTMP3);
		PHY_TESTRES.CS_AVG_TOTAL = divu16round(PHY_TESTRES.CS_AVG_TOTAL, (uint16_t)PHY_TEST_COUNTTMP3);
	} else if(PHY_TEST_COUNTTMP2 > 0) {
		PHY_TESTRES.FO_AVG_TOTAL = divs16round(PHY_TESTRES.FO_AVG, (int16_t)PHY_TEST_COUNTTMP2);
		PHY_TESTRES.ED_AVG_TOTAL = divu16round(PHY_TESTRES.ED_AVG, (uint16_t)PHY_TEST_COUNTTMP2);
		PHY_TESTRES.CS_AVG_TOTAL = divu16round(PHY_TESTRES.CS_AVG, (uint16_t)PHY_TEST_COUNTTMP2);
	} else {
		PHY_TESTRES.FO_AVG_TOTAL = 0;
		PHY_TESTRES.ED_AVG_TOTAL = 0;
		PHY_TESTRES.CS_AVG_TOTAL = 0;
	}
} // End of PHYTestStatistics3()


/******************************************************************************/
/***************************************************************************//**
 * \brief Division of Unsigned 16-Bit Values with Rounding
 *******************************************************************************
 * \param va - Dividend
 * \param vb - Divisor
 *******************************************************************************
 * \return Result
 *******************************************************************************
 ******************************************************************************/
uint16_t divu16round(uint16_t va, uint16_t vb)
{
	uint16_t vr;
	vr = va / vb;
	/* positive truncated down, check if needs correction */
	if ((va % vb) > (vb / 2))
		vr += 1;

	return vr;
} // End of divu16round()


/******************************************************************************/
/***************************************************************************//**
 * \brief Division of Signed 16-Bit Values with Rounding
 *******************************************************************************
 * \param va - Dividend
 * \param vb - Divisor
 *******************************************************************************
 * \return Result
 *******************************************************************************
 ******************************************************************************/
int16_t divs16round(int16_t va, int16_t vb)
{
	int16_t vr;

	vr = va / vb;
	if (vr > 0) {
		/* positive truncated down, check if needs correction */
		if ((va % vb) > (vb / 2)) {
			vr += 1;
		}
	} else {
		/* negative truncated up, check if needs correction */
		if ((va % vb) < (-vb / 2)) {
			vr -= 1;
		}
	}
	return vr;
} // End of divs16round()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Testmode Exit
 *******************************************************************************
 * \param status - Status to be reported
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportExit(uint8_t status, struct ca821x_dev *pDeviceRef)
{
	printf("PHY Testmode Exit. Status: ");
	switch (status) {
	case MAC_SUCCESS:
		printf("Success");
		break;
	case MAC_SYSTEM_ERROR:
		printf("MAC System Error");
		break;
	case MAC_CHANNEL_ACCESS_FAILURE:
		printf("MAC Channel Access Failure");
		break;
	case MAC_NO_ACK:
		printf("MAC no ACK");
		break;
	default:
		printf("Unknown: %#03x", status);
		break;
	}
	printf("\n");
} // End of PHYTestReportExit()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Test Mode after Initialisation is done
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportTestMode(struct ca821x_dev *pDeviceRef)
{
	printf("PHY Testmode initialised for ");
	switch (PHY_TESTMODE & PHY_TEST_CMD) {
	case PHY_TEST_OFF:      printf("Normal Mode");          break;
	case PHY_TEST_TX_PKT:   printf("Tx (Packets)");         break;
	case PHY_TEST_TX_CONT:  printf("Tx (Continuous)");      break;
	case PHY_TEST_RX_PER:   printf("Rx (PER Test)");        break;
	case PHY_TEST_RX_PSN:   printf("Rx (Packet Sniffer)");  break;
	case PHY_TEST_RX_EDSN:  printf("Rx (Energy Sniffer)");  break;
	case PHY_TEST_LO_1:     printf("LO1 (Closed Loop)");    break;
	case PHY_TEST_LO_2:     printf("LO2 (Open Loop)");      break;
	case PHY_TEST_LO_3:     printf("LO3 (Locking Test)");   break;
	default:                printf("Unknown Test Mode");    break;
	}
	printf("\n");
} // End of PHYTestReportTestMode()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Test Parameter PIB
 *******************************************************************************
 * \param parameter - Specific Parameter or PHY_TESTPAR_ALL
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportTestParameters(uint8_t parameter, struct ca821x_dev *pDeviceRef)
{
	if (parameter == PHY_TESTPAR_ALL) {
		printf("PHY Test Parameter Set:\n");
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_PACKETPERIOD)) {
		printf("PACKETPERIOD   = %u ms\n", PHY_TESTPAR.PACKETPERIOD);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_PACKETLENGTH)) {
		printf("PACKETLENGTH   = %u Bytes\n", PHY_TESTPAR.PACKETLENGTH);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_NUMBEROFPKTS)) {
		printf("NUMBEROFPKTS   = %u\n", PHY_TESTPAR.NUMBEROFPKTS);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_PACKETDATATYPE)) {
		printf("PACKETDATATYPE = ");
		switch (PHY_TESTPAR.PACKETDATATYPE) {
		case TDME_TXD_RANDOM:    printf("Random");                      break;
		case TDME_TXD_SEQRANDOM: printf("Random with Sequence Number"); break;
		case TDME_TXD_COUNT:     printf("Count (Increment)");           break;
		case TDME_TXD_APPENDED:  printf("Appended");                    break;
		default:                 printf("???");                         break;
		}
		printf("\n");
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_CHANNEL)) {
		printf(
			"CHANNEL        = %u (%#03x)\n",
			PHY_TESTPAR.CHANNEL,
			PHY_TESTPAR.CHANNEL
		);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_TXPOWER)) {
		printf(
			"TXPOWER        = IB: %u; PB: %u; BOOST: %u\n",
			PHY_TESTPAR.TXPOWER_IB,
			PHY_TESTPAR.TXPOWER_PB,
			PHY_TESTPAR.TXPOWER_BOOST
		);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_EDTHRESHOLD)) {
		printf("EDTHRESHOLD    = %u\n", PHY_TESTPAR.EDTHRESHOLD);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_RX_FFSYNC)) {
		printf("RX_FFSYNC      = %u (", PHY_TESTPAR.RX_FFSYNC);
		if (PHY_TESTPAR.RX_FFSYNC == 0) {
			printf("Off");
		} else {
			printf("On");
		}
		printf(")\n");
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_1_RXTXB)) {
		printf("LO_1_RXTXB     = %u (", PHY_TESTPAR.LO_1_RXTXB);
		if(PHY_TESTPAR.LO_1_RXTXB == 0) {
			printf("Tx");
		} else {
			printf("Rx");
		}
		printf(")\n");
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_2_FDAC)) {
		printf(
			"LO_2_FDAC      = %u (%#03x)\n",
			PHY_TESTPAR.LO_2_FDAC,
			PHY_TESTPAR.LO_2_FDAC
		);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_3_LOCKS)) {
		printf("LO_3_LOCKS     = %u\n", PHY_TESTPAR.LO_3_LOCKS);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_LO_3_PERIOD)) {
		printf("LO_3_PERIOD    = %u\n", PHY_TESTPAR.LO_3_PERIOD);
	}
	if ((parameter == PHY_TESTPAR_ALL) || (parameter == PHY_TESTPAR_ATM)) {
		printf(
			"ATM            = %u (%#03x)\n",
			PHY_TESTPAR.ATM,
			PHY_TESTPAR.ATM
		);
	}
} // End of PHYTestReportTestParameters()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Successful Transmission of Packet
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportPacketTransmitted(struct ca821x_dev *pDeviceRef)
{
	uint8_t i;

	if (PHYBuffer[0] == TDME_LO_ERROR) {
		printf("Tx: LO Error");
	} else {
		printf("Tx: SN=%3u PL=%3u:", PHYBuffer[1], PHYLength);
		for (i = 0; i < PHYLength - 2; ++i) {
			printf(" %02X", PHYBuffer[i+2]);
		}
	}
	printf("\n");
} // End of PHYTestReportPacketTransmitted()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Packet Transmission Analysis
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportTransmitPacketAnalysis(struct ca821x_dev *pDeviceRef)
{
	printf("Tx: %u Packets sent\n", PHY_TESTRES.PACKET_COUNT);
	if(PHY_TESTPAR.MACENABLED) {
		printf(
			"%u No-Acks; %u Channel Access Failures\n",
			PHY_TESTRES.SHRERR_COUNT,
			PHY_TESTRES.PHRERR_COUNT
		);
	}
} // End of PHYTestReportTransmitPacketAnalysis()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Reception of Packet
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportPacketReceived(struct ca821x_dev *pDeviceRef)
{
	uint8_t i;
	int8_t sv;

	if (PHYBuffer[0] == TDME_LO_ERROR) {
		printf("Rx: LO Error");
	} else {
		printf("Rx: PL=%3u:", PHYLength);
		for (i = 0; i < PHYLength-2; ++i) {
			printf(" %02X", PHYBuffer[i+4]);
		}
		if(        PHYBuffer[0] == TDME_FCS_ERROR) {
			printf("; CRCErr");
		} else if (PHYBuffer[0] == TDME_SHR_ERROR) {
			printf("; SHRErr");
		} else if (PHYBuffer[0] == TDME_PHR_ERROR) {
			printf("; PHRErr");
		}
		sv = (int8_t)PHYBuffer[3];
		printf("; ED: %u; CS: %u; FO: %d", PHYBuffer[1], PHYBuffer[2], sv);
	}
	printf("\n");
} // End of PHYTestReportPacketReceived()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Packet Reception Analysis
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportReceivedPacketAnalysis(struct ca821x_dev *pDeviceRef)
{
	uint32_t errcount_total;

	errcount_total = PHY_TESTRES.CRCERR_COUNT
	               + PHY_TESTRES.PREERR_COUNT;

	printf(
		"Rx: %u Packets received, %u Errors\n",
		PHY_TESTRES.PACKET_COUNT,
		errcount_total
	);

	if (PHY_TESTPAR.PACKETPERIOD != 0) {
		if (PHY_TESTPAR.RX_FFSYNC) {
			printf(
				"Rx Error  Analysis: CRC: %u; PHR: %u; SHR: %u; PRE: %u\n",
				PHY_TESTRES.CRCERR_COUNT,
				PHY_TESTRES.PHRERR_COUNT,
				PHY_TESTRES.SHRERR_COUNT,
				PHY_TESTRES.PREERR_COUNT
			);
		} else {
			printf(
				"Rx Error  Analysis: CRC: %u; Missed: %u\n",
				PHY_TESTRES.CRCERR_COUNT,
				PHY_TESTRES.PREERR_COUNT
			);
		}
	}

	if (!PHY_TEST_FLAG) { /* don't report if missed packet */
		printf(
			"Rx Signal Analysis: ED: %u; CS: %u; FO: %d\n",
			PHY_TESTRES.ED_AVG,
			PHY_TESTRES.CS_AVG,
			PHY_TESTRES.FO_AVG
		);
	}
} // End of PHYTestReportReceivedPacketAnalysis()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Complete Packet Error Rate (PER) Test Result
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportPERTestResult(struct ca821x_dev *pDeviceRef)
{
	uint32_t errcount_total;

	errcount_total = PHY_TESTRES.CRCERR_COUNT
	               + PHY_TESTRES.PREERR_COUNT;

	printf(
		"PER Test Result: %u Packets received, %u Errors\n",
		PHY_TESTRES.PACKET_COUNT,
		errcount_total
	);
	if (PHY_TESTPAR.RX_FFSYNC) {
		printf(
			"PER Error Analysis: CRC: %u; PHR: %u; SHR: %u; PRE: %u\n",
			PHY_TESTRES.CRCERR_COUNT,
			PHY_TESTRES.PHRERR_COUNT,
			PHY_TESTRES.SHRERR_COUNT,
			PHY_TESTRES.PREERR_COUNT
		);
	} else {
		printf(
			"PER Error Analysis: CRC: %u; Missed: %u\n",
			PHY_TESTRES.CRCERR_COUNT,
			PHY_TESTRES.PREERR_COUNT
		);
	}
	printf(
		"Rx Overall Averages: ED: %u; CS: %u; FO: %d\n",
		PHY_TESTRES.ED_AVG_TOTAL,
		PHY_TESTRES.CS_AVG_TOTAL,
		PHY_TESTRES.FO_AVG_TOTAL
	);
	printf(
		"Rx Min/Max Analysis: ED Max: %u; ED Min: %u; CS Max: %u; CS Min: %u\n",
		PHY_TESTRES.ED_MAX,
		PHY_TESTRES.ED_MIN,
		PHY_TESTRES.CS_MAX,
		PHY_TESTRES.CS_MIN
	);
	if (PHY_TESTPAR.RX_FFSYNC) {
		printf(
			"Rx Missed Pkt Analysis: SHR: %u; PHR: %u\n",
			PHY_TESTRES.SHRERR_COUNT,
			PHY_TESTRES.PHRERR_COUNT
		);
	}
	printf("PER Test completed\n");
} // End of PHYTestReportPERTestResult()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report Reception of ED above Threshold
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportEDReceived(struct ca821x_dev *pDeviceRef)
{
	uint16_t tat;

	tat = ((uint16_t)(PHYBuffer[5]) << 8)
	    +  (uint16_t)(PHYBuffer[4]);

	printf(
		"ED above %3u: ED=%3u CS=%3u THigh=%5u N=%u\n",
		PHYBuffer[1],
		PHYBuffer[2],
		PHYBuffer[3],
		tat,
		PHY_TESTRES.PACKET_COUNT
	);
} // End of PHYTestReportEDReceived()


/******************************************************************************/
/***************************************************************************//**
 * \brief Report LO Test 3 Result (Locking Test)
 *******************************************************************************
 * \param ntest - Number of Test
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestReportLOLocking(uint8_t ntest, struct ca821x_dev *pDeviceRef)
{
	if (PHYBuffer[2]) {
		printf("Rx");
	} else {
		printf("Tx");
	}
	printf(
		" Ch=%3u N=%3u FDAC=%3u AMP=%3u",
		PHYBuffer[1],
		ntest,
		PHYBuffer[3],
		PHYBuffer[4]
	);
	if (!PHYBuffer[2]) {
		printf(" TXCAL=%3u", PHYBuffer[5]);
	}
	if(PHYBuffer[0] == TDME_LO_ERROR) {
		printf(" LOCK FAILURE");
	}
	printf("\n");
} // End of PHYTestReportLOLocking()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Change Test Configuration
 *******************************************************************************
 * \param val - Value to set the Configurations to
 * \param pDeviceRef - Device reference
 *******************************************************************************
 ******************************************************************************/
void PHYTestCfg(uint8_t val, struct ca821x_dev *pDeviceRef)
{
	if (val == 0) {
		printf("PHYTest configured to PHY only\n");
		PHY_TESTPAR.MACENABLED = 0;
	} else if (val == 1) {
		printf("PHYTest configured to MAC Usage\n");
		PHY_TESTPAR.MACENABLED = 1;
	} else {
		printf("PHYTest Configuration unknown\n");
	}
} // End of PHYTestCfg()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Wrapper for TDME_TESTMODE_request_sync
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 ******************************************************************************/
uint8_t PHY_TESTMODE_request(struct ca821x_dev *pDeviceRef)
{
	uint8_t status;
	uint8_t tdme_testmode;

	switch (PHY_TESTMODE & PHY_TEST_CMD) {
		case PHY_TEST_OFF:      tdme_testmode = TDME_TEST_OFF;   break;
		case PHY_TEST_TX_PKT:   tdme_testmode = TDME_TEST_TX;    break;
		case PHY_TEST_TX_CONT:  tdme_testmode = TDME_TEST_TX;    break;
		case PHY_TEST_RX_PER:   tdme_testmode = TDME_TEST_RX;    break;
		case PHY_TEST_RX_PSN:   tdme_testmode = TDME_TEST_RX;    break;
		case PHY_TEST_RX_EDSN:  tdme_testmode = TDME_TEST_ED;    break;
		case PHY_TEST_LO_1:     tdme_testmode = TDME_TEST_LO_1;  break;
		case PHY_TEST_LO_2:     tdme_testmode = TDME_TEST_LO_2;  break;
		case PHY_TEST_LO_3:     tdme_testmode = TDME_TEST_LO_3;  break;
		default:                tdme_testmode = TDME_TEST_OFF;   break;
	}
	status = TDME_TESTMODE_request_sync(tdme_testmode, pDeviceRef);
	return status;
} // End of PHY_TESTMODE_request()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Wrapper for TDME_SET_request_sync
 *******************************************************************************
 * \param attribute - TDME Attribute
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 ******************************************************************************/
uint8_t PHY_SET_request(uint8_t attribute, struct ca821x_dev *pDeviceRef)
{
	uint8_t status;

	switch (attribute) {
	case TDME_CHANNEL:
		PHYLength    =  1;
		PHYBuffer[0] =  PHY_TESTPAR.CHANNEL;
		break;
	case TDME_TX_CONFIG:
		PHYLength    =  4;
		PHYBuffer[0] =  PHY_TESTPAR.TXPOWER_IB;
		PHYBuffer[1] =  PHY_TESTPAR.TXPOWER_PB;
		PHYBuffer[2] =  PHY_TESTPAR.TXPOWER_BOOST;
		PHYBuffer[3] =  PHY_TESTPAR.TXCONT;
		break;
	case TDME_ED_CONFIG:
		PHYLength    =  1;
		PHYBuffer[0] =  PHY_TESTPAR.EDTHRESHOLD;
		break;
	case TDME_RX_CONFIG:
		PHYLength    =  1;
		PHYBuffer[0] =  PHY_TESTPAR.RX_FFSYNC;
		break;
	case TDME_LO_1_CONFIG:
		PHYLength    =  2;
		PHYBuffer[0] =  PHY_TESTPAR.LO_1_RXTXB;
		PHYBuffer[1] =  PHY_TESTPAR.CHANNEL;
		break;
	case TDME_LO_2_CONFIG:
		PHYLength    =  1;
		PHYBuffer[0] =  PHY_TESTPAR.LO_2_FDAC;
		break;
	case TDME_ATM_CONFIG:
		PHYLength    =  1;
		PHYBuffer[0] =  PHY_TESTPAR.ATM;
		break;
	default:
		PHYLength    =  1;
		PHYBuffer[0] =  0x00;
		break;
	}
	status = TDME_SET_request_sync(attribute, PHYLength, PHYBuffer, pDeviceRef);
	return status;
} // End of PHY_SET_request()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Wrapper for TDME_TXPKT_request_sync
 *******************************************************************************
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 ******************************************************************************/
uint8_t PHY_TXPKT_request(struct ca821x_dev *pDeviceRef)
{
	uint8_t i;
	uint8_t status;

	PHYLength    =  PHY_TESTPAR.PACKETLENGTH;
	PHYBuffer[1] =  PHY_TESTRES.SEQUENCENUMBER;

	if (PHY_TESTPAR.PACKETDATATYPE == TDME_TXD_APPENDED) {
		for (i = 0; i < PHY_TESTPAR.PACKETLENGTH; ++i) {
			PHYBuffer[i+2] = 0x00; /* currently filled with 0's */
		}
	}

	status = TDME_TXPKT_request_sync(
		PHY_TESTPAR.PACKETDATATYPE,
		&PHYBuffer[1],
		&PHYLength,
		PHYBuffer+2,
		pDeviceRef
	);
	PHYBuffer[0] = status;
	++PHY_TESTRES.SEQUENCENUMBER;
	return status;
} // End of PHY_TXPKT_request()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Wrapper for TDME_LOTLK_request_sync
 *******************************************************************************
 * \param ch - 802.15.4 Channel to be tested
 * \param rx_txb - rx Mode to be tested when 1, Tx Mode when 0
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 ******************************************************************************/
uint8_t PHY_LOTLK_request(uint8_t ch, uint8_t rx_txb, struct ca821x_dev *pDeviceRef)
{
	uint8_t status;

	PHYBuffer[1] = ch;
	PHYBuffer[2] = rx_txb;

	status = TDME_LOTLK_request_sync(
		&PHYBuffer[1],
		&PHYBuffer[2],
		&PHYBuffer[3],
		&PHYBuffer[4],
		&PHYBuffer[5],
		pDeviceRef
	);
	PHYBuffer[0] = status;
	return status;
} // End of PHY_LOTLK_request()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Wrapper for TDME_RXPKT_indication
 *******************************************************************************
 * \param indication - rxpkt indication buffer
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 ******************************************************************************/
int PHY_RXPKT_indication(struct MAC_Message *indication, struct ca821x_dev *pDeviceRef)
{
	int status = indication->PData.TDMERxPktInd.Status;

	PHYBuffer[0] = status;
	PHYBuffer[1] = indication->PData.TDMERxPktInd.TestPacketEDValue;
	PHYBuffer[2] = indication->PData.TDMERxPktInd.TestPacketCSValue;
	PHYBuffer[3] = indication->PData.TDMERxPktInd.TestPacketFoffsValue;
	PHYLength = indication->PData.TDMERxPktInd.TestPacketLength;
	memcpy(
		&PHYBuffer[4],
		indication->PData.TDMERxPktInd.TestPacketData,
		indication->PData.TDMERxPktInd.TestPacketLength
	);

	switch (status) {
	case TDME_LO_ERROR:   ++PHY_TESTRES.LOERR_COUNT;   break;
	case TDME_FCS_ERROR:  ++PHY_TESTRES.CRCERR_COUNT;  break;
	case TDME_SHR_ERROR:  ++PHY_TESTRES.SHRERR_COUNT;  break;
	case TDME_PHR_ERROR:  ++PHY_TESTRES.PHRERR_COUNT;  break;
	default:                                           break;
	}

	PHY_TESTRES.PACKET_RECEIVED = 1; /* Flag indication */
	return status;
} // End of PHY_RXPKT_indication()


/******************************************************************************/
/***************************************************************************//**
 * \brief PHY Test Wrapper for TDME_EDDET_indication
 *******************************************************************************
 * \param indication - eddet indication buffer
 * \param pDeviceRef - Device reference
 *******************************************************************************
 * \return Status
 *******************************************************************************
 ******************************************************************************/
int PHY_EDDET_indication(struct MAC_Message *indication, struct ca821x_dev *pDeviceRef)
{
	int status = 0;

	PHYBuffer[1] = indication->PData.TDMEEDDetInd.TestEDThreshold;
	PHYBuffer[2] = indication->PData.TDMEEDDetInd.TestEDValue;
	PHYBuffer[3] = indication->PData.TDMEEDDetInd.TestCSValue;
	PHYBuffer[4] = indication->PData.TDMEEDDetInd.TestTimeAboveThreshold_us[0];
	PHYBuffer[5] = indication->PData.TDMEEDDetInd.TestTimeAboveThreshold_us[1];

	PHY_TESTRES.PACKET_RECEIVED = 1; /* Flag indication */
	return status;
} // End of PHY_EDDET_indication()
