/*
 * Copyright (c) 2016, Cascoda
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef IEEE_802_15_4_H
#define IEEE_802_15_4_H


/******************************************************************************/
/****** MAC Status Enumerations                                          ******/
/******************************************************************************/
#define MAC_SUCCESS                     (0x00)
#define MAC_ERROR                       (0x01)
#define MAC_CANCELLED                   (0x02)
#define MAC_READY_FOR_POLL              (0x03)
#define MAC_COUNTER_ERROR               (0xDB)
#define MAC_IMPROPER_KEY_TYPE           (0xDC)
#define MAC_IMPROPER_SECURITY_LEVEL     (0xDD)
#define MAC_UNSUPPORTED_LEGACY          (0xDE)
#define MAC_UNSUPPORTED_SECURITY        (0xDF)
#define MAC_BEACON_LOST                 (0xE0)
#define MAC_CHANNEL_ACCESS_FAILURE      (0xE1)
#define MAC_DENIED                      (0xE2)
#define MAC_DISABLE_TRX_FAILURE         (0xE3)
#define MAC_SECURITY_ERROR              (0xE4)
#define MAC_FRAME_TOO_LONG              (0xE5)
#define MAC_INVALID_GTS                 (0xE6)
#define MAC_INVALID_HANDLE              (0xE7)
#define MAC_INVALID_PARAMETER           (0xE8)
#define MAC_NO_ACK                      (0xE9)
#define MAC_NO_BEACON                   (0xEA)
#define MAC_NO_DATA                     (0xEB)
#define MAC_NO_SHORT_ADDRESS            (0xEC)
#define MAC_OUT_OF_CAP                  (0xED)
#define MAC_PAN_ID_CONFLICT             (0xEE)
#define MAC_REALIGNMENT                 (0xEF)
#define MAC_TRANSACTION_EXPIRED         (0xF0)
#define MAC_TRANSACTION_OVERFLOW        (0xF1)
#define MAC_TX_ACTIVE                   (0xF2)
#define MAC_UNAVAILABLE_KEY             (0xF3)
#define MAC_UNSUPPORTED_ATTRIBUTE       (0xF4)
#define MAC_INVALID_ADDRESS             (0xF5)
#define MAC_ON_TIME_TOO_LONG            (0xF6)
#define MAC_PAST_TIME                   (0xF7)
#define MAC_TRACKING_OFF                (0xF8)
#define MAC_INVALID_INDEX               (0xF9)
#define MAC_LIMIT_REACHED               (0xFA)
#define MAC_READ_ONLY                   (0xFB)
#define MAC_SCAN_IN_PROGRESS            (0xFC)
#define MAC_SUPERFRAME_OVERLAP          (0xFD)
#define MAC_SYSTEM_ERROR                (0xFF)


/******************************************************************************/
/****** MAC Address Mode Definitions                                     ******/
/******************************************************************************/
#define MAC_MODE_NO_ADDR                ((unsigned)0x00)
#define MAC_MODE_RESERVED               ((unsigned)0x01)
#define MAC_MODE_SHORT_ADDR             ((unsigned)0x02)
#define MAC_MODE_LONG_ADDR              ((unsigned)0x03)


/******************************************************************************/
/****** MAC Address Code Definitions                                     ******/
/******************************************************************************/
// Made from Combinations of Source and Destination Addressing Modes
#define AC_INVALID                      (0xFF)
#define AC_DP0DA0SP2SA2                 (0)
#define AC_DP0DA0SP2SA8                 (1)
#define AC_DP2DA2SP0SA0                 (2)
#define AC_DP2DA8SP0SA0                 (3)
#define AC_DP2DA2SP0SA2                 (4)
#define AC_DP2DA8SP0SA2                 (5)
#define AC_DP2DA2SP0SA8                 (6)
#define AC_DP2DA8SP0SA8                 (7)
#define AC_DP2DA2SP2SA2                 (8)
#define AC_DP2DA8SP2SA2                 (9)
#define AC_DP2DA2SP2SA8                 (10)
#define AC_DP2DA8SP2SA8                 (11)


/******************************************************************************/
/****** MAC Frame Control Field Definitions                              ******/
/******************************************************************************/
#define MAC_FC_FT_MASK                  (0x0003)
#define MAC_FC_FT_BEACON                (0x0000)
#define MAC_FC_FT_DATA                  (0x0001)
#define MAC_FC_FT_ACK                   (0x0002)
#define MAC_FC_FT_COMMAND               (0x0003)

#define MAC_FC_SEC_ENA                  (0x0008)
#define MAC_FC_FP                       (0x0010)
#define MAC_FC_ACK_REQ                  (0x0020)
#define MAC_FC_PAN_COMP                 (0x0040)
#define MAC_FC_DAM_SHORT                (MAC_MODE_SHORT_ADDR<<10)
#define MAC_FC_DAM_LONG                 (MAC_MODE_LONG_ADDR<<10)
#define MAC_FC_VER2006                  (0x1000)
#define MAC_FC_VER(fc)                  ((fc>>12)&3)
#define MAC_FC_SAM_SHORT                (MAC_MODE_SHORT_ADDR<<14)
#define MAC_FC_SAM_LONG                 (MAC_MODE_LONG_ADDR<<14)
#define MAC_FC_DAM(fc)                  ((fc>>10)&3)
#define MAC_FC_SAM(fc)                  ((fc>>14)&3)


/******************************************************************************/
/****** MAC TxOption Bits Definitions                                    ******/
/******************************************************************************/
#define TXOPT_ACKREQ                    (0x01)
#define TXOPT_GTS                       (0x02)
#define TXOPT_INDIRECT                  (0x04)


/******************************************************************************/
/****** MAC ScanType Parameter for MLME_SCAN Request and Confirm         ******/
/******************************************************************************/
#define ENERGY_DETECT                   (0x00)
#define ACTIVE_SCAN                     (0x01)
#define PASSIVE_SCAN                    (0x02)
#define ORPHAN_SCAN                     (0x03)


/******************************************************************************/
/****** MAC Command Frame Identifiers                                    ******/
/******************************************************************************/
#define CMD_ASSOCIATION_REQ         (1)
#define CMD_ASSOCIATION_RSP         (2)
#define CMD_DISASSOCIATION_NFY      (3)
#define CMD_DATA_REQ                (4)
#define CMD_PANID_CONFLICT_NFY      (5)
#define CMD_ORPHAN_NFY              (6)
#define CMD_BEACON_REQ              (7)
#define CMD_COORD_REALIGN           (8)
#define CMD_GTS_REQ                 (9)


/******************************************************************************/
/****** PHY/MAC PIB Attribute Enumerations                               ******/
/******************************************************************************/
#define phyCurrentChannel               (0x00)
#define phyChannelsSupport              (0x01)
#define phyTransmitPower                (0x02)
#define phyCCAMode                      (0x03)
#define phyCurrentPage                  (0x04)
#define phyMaxFrameDuration             (0x05)
#define phySHRDuration                  (0x06)
#define phySymbolsPerOctet              (0x07)

#define phyPibFirst                     (phyCurrentChannel)
#define phyPibLast                      (phySymbolsPerOctet)

#define macAckWaitDuration              (0x40)
#define macAssociationPermit            (0x41)
#define macAutoRequest                  (0x42)
#define macBattLifeExt                  (0x43)
#define macBattLifeExtPeriods           (0x44)
#define macBeaconPayload                (0x45)
#define macBeaconPayloadLength          (0x46)
#define macBeaconOrder                  (0x47)
#define macBeaconTxTime                 (0x48)
#define macBSN                          (0x49)
#define macCoordExtendedAddress         (0x4a)
#define macCoordShortAddress            (0x4b)
#define macDSN                          (0x4c)
#define macGTSPermit                    (0x4d)
#define macMaxCSMABackoffs              (0x4e)
#define macMinBE                        (0x4f)
#define macPANId                        (0x50)
#define macPromiscuousMode              (0x51)
#define macRxOnWhenIdle                 (0x52)
#define macShortAddress                 (0x53)
#define macSuperframeOrder              (0x54)
#define macTransactionPersistenceTime	(0x55)
#define macAssociatedPANCoord           (0x56)
#define macMaxBE                        (0x57)
#define macMaxFrameTotalWaitTime        (0x58)
#define macMaxFrameRetries              (0x59)
#define macResponseWaitTime             (0x5A)
#define macSyncSymbolOffset             (0x5B)
#define macTimestampSupported           (0x5C)
#define macSecurityEnabled              (0x5D)

#define macPibFirst                     (macAckWaitDuration)
#define macPibLast                      (macSecurityEnabled)

#define macKeyTable                     (0x71)
#define macKeyTableEntries              (0x72)
#define macDeviceTable                  (0x73)
#define macDeviceTableEntries           (0x74)
#define macSecurityLevelTable           (0x75)
#define macSecurityLevelTableEntries    (0x76)
#define macFrameCounter                 (0x77)
#define macAutoRequestSecurityLevel     (0x78)
#define macAutoRequestKeyIdMode         (0x79)
#define macAutoRequestKeySource         (0x7A)
#define macAutoRequestKeyIndex          (0x7B)
#define macDefaultKeySource             (0x7C)
#define macPANCoordExtendedAddress      (0x7D)
#define macPANCoordShortAddress         (0x7E)

#define macSecPibFirst                  (macKeyTable)
#define macSecPibLast                   (macPANCoordShortAddress)

#define nsIEEEAddress                   (0xFF)    /* Non-standard IEEE address */


/******************************************************************************/
/****** PHY/MAC Constants                                                ******/
/******************************************************************************/
#define aMaxPHYPacketSize               (127)
#define aMaxMACSafePayloadSize          (102)
#define aTurnaroundTime                 (12)
#define aSymbolPeriod_us                (16)
#define aNumSuperframeSlots             (16)

#define aBaseSlotDuration               (60)
#define aBaseSuperframeDuration         (aBaseSlotDuration*aNumSuperframeSlots)
#define aMaxBeaconOverhead              (75)
#define aMaxBeaconPayloadLength         (aMaxPHYPacketSize-aMaxBeaconOverhead)
#define aUnitBackoffPeriod              (20)

#define MAX_ATTRIBUTE_SIZE              (250)
#define MAX_DATA_SIZE                   (114)

#define M_MinimumChannel                (11)
#define M_MaximumChannel                (26)
#define M_ValidChannels                 (0x07FFF800)

#define MAX_FRAME_DURATION              (266)


/******************************************************************************/
/****** Length Definitions for different Commands                        ******/
/******************************************************************************/
// Data Request includes 1 phylen, 2 fc, 1 dsn, 12 address, 1 payload, 2 fcs
#define BASELEN_DATA_REQUEST            (19)


#endif  // IEEE_802_15_4_H
