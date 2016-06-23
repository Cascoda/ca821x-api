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


#ifndef MAC_MESSAGES_H
#define MAC_MESSAGES_H

#include <stdint.h>

#include "ieee_802_15_4.h"
#include "hwme_tdme.h"

#define MAX_HWME_ATTRIBUTE_SIZE  16
#define MAX_TDME_ATTRIBUTE_SIZE  2


/******************************************************************************/
/****** Structures used in the MAC MCPS/MLME Procedure Definitions       ******/
/******************************************************************************/

struct FullAddr {
	uint8_t         AddressMode;
	uint8_t         PANId[2];
	uint8_t         Address[8];
};

union MacAddr {
	uint16_t        ShortAddress;
	uint8_t         IEEEAddress[8];
};

struct SecSpec {
	uint8_t         SecurityLevel;
	uint8_t         KeyIdMode;
	uint8_t         KeySource[8];
	uint8_t         KeyIndex;
};

struct PanDescriptor {
	struct FullAddr Coord;
	uint8_t         LogicalChannel;
	uint8_t         SuperframeSpec[2];
	uint8_t         GTSPermit;
	uint8_t         LinkQuality;
	uint8_t         TimeStamp[4];
	uint8_t         SecurityFailure;
	struct SecSpec  Security;
};


/******************************************************************************/
/****** Downlink Functions Parameter Set Definitions                     ******/
/******************************************************************************/

// MCPS

struct MCPS_DATA_request_pset {
	uint8_t         SrcAddrMode;
	struct FullAddr Dst;
	uint8_t         MsduLength;
	uint8_t         MsduHandle;
	uint8_t         TxOptions;
	uint8_t         Msdu[MAX_DATA_SIZE];
};

// MLME

struct MLME_ASSOCIATE_request_pset {
	uint8_t         LogicalChannel;
	struct FullAddr Dst;
	uint8_t         CapabilityInfo;
	struct SecSpec  Security;
};

struct MLME_ASSOCIATE_response_pset {
	uint8_t         DeviceAddress[8];
	uint8_t         AssocShortAddress[2];
	uint8_t         Status;
	struct SecSpec  Security;
};

struct MLME_DISASSOCIATE_request_pset {
	struct FullAddr DevAddr;
	uint8_t         DisassociateReason;
	uint8_t         TxIndirect;
 	struct SecSpec  Security;
};

struct MLME_GET_request_pset {
	uint8_t         PIBAttribute;
	uint8_t         PIBAttributeIndex;
};

struct MLME_ORPHAN_response_pset {
	uint8_t         OrphanAddress[8];
	uint8_t         ShortAddress[2];
	uint8_t         AssociatedMember;
	struct SecSpec  Security;
};

struct MLME_POLL_request_pset {
	struct FullAddr CoordAddress;
	uint8_t         Interval[2];      /* polling interval in 0.1 seconds res */
	                                  /* 0 means poll once */
	                                  /* 0xFFFF means stop polling */
	struct SecSpec  Security;
};

struct MLME_RX_ENABLE_request_pset {
	uint8_t         DeferPermit;
	uint8_t         RxOnTime[4];
	uint8_t         RxOnDuration[4];
};

struct MLME_SCAN_request_pset {
	uint8_t         ScanType;
	uint8_t         ScanChannels[4];
	uint8_t         ScanDuration;
	struct SecSpec  Security;
};

struct MLME_SET_request_pset {
	uint8_t         PIBAttribute;
	uint8_t         PIBAttributeIndex;
	uint8_t         PIBAttributeLength;
	uint8_t         PIBAttributeValue[MAX_ATTRIBUTE_SIZE];
};

struct MLME_START_request_pset {
	uint8_t         PANId[2];
	uint8_t         LogicalChannel;
	uint8_t         BeaconOrder;
	uint8_t         SuperframeOrder;
	uint8_t         PANCoordinator;
	uint8_t         BatteryLifeExtension;
	uint8_t         CoordRealignment;
	struct SecSpec  CoordRealignSecurity;
	struct SecSpec  BeaconSecurity;
};

// HWME

struct HWME_SET_request_pset {
	uint8_t         HWAttribute;
	uint8_t         HWAttributeLength;
	uint8_t         HWAttributeValue[MAX_HWME_ATTRIBUTE_SIZE];
};

struct HWME_GET_request_pset {
	uint8_t         HWAttribute;
};

struct HWME_HAES_request_pset {
	uint8_t            HAESMode;
	uint8_t            HAESData[16];
};

// TDME

struct TDME_SETSFR_request_pset {
	uint8_t         SFRPage;
	uint8_t         SFRAddress;
	uint8_t         SFRValue;
};

struct TDME_GETSFR_request_pset {
	uint8_t         SFRPage;
	uint8_t         SFRAddress;
};

struct TDME_TESTMODE_request_pset {
	uint8_t            TestMode;
};

struct TDME_SET_request_pset {
	uint8_t         TDAttribute;
	uint8_t         TDAttributeLength;
	uint8_t         TDAttributeValue[MAX_TDME_ATTRIBUTE_SIZE];
};

struct TDME_TXPKT_request_pset {
	uint8_t            TestPacketDataType;
	uint8_t            TestPacketSequenceNumber;
	uint8_t            TestPacketLength;
	uint8_t            TestPacketData[128];
};

struct TDME_LOTLK_request_pset {
	uint8_t            TestChannel;
	uint8_t            TestRxTxb;
};


/******************************************************************************/
/****** Uplink Functions Parameter Set Definitions                       ******/
/******************************************************************************/

// MCPS

struct MCPS_DATA_confirm_pset {
	uint8_t            MsduHandle;
	uint8_t            Status;
	uint8_t            TimeStamp[4];
};

struct MCPS_PURGE_confirm_pset {
	uint8_t            MsduHandle;
	uint8_t            Status;
};

struct MCPS_DATA_indication_pset {
    struct FullAddr    Src;
    struct FullAddr    Dst;
    uint8_t            MsduLength;
    uint8_t            MpduLinkQuality;
    uint8_t            DSN;
    uint8_t            TimeStamp[4];
    uint8_t            Msdu[MAX_DATA_SIZE];
};

// MLME

struct MLME_ASSOCIATE_indication_pset {
	uint8_t            DeviceAddress[8];
	uint8_t            CapabilityInformation;
	struct SecSpec     Security;
};

struct MLME_ASSOCIATE_confirm_pset {
	uint8_t            AssocShortAddress[2];
	uint8_t            Status;
	struct SecSpec     Security;
};

struct MLME_DISASSOCIATE_confirm_pset {
	uint8_t            Status;
	struct FullAddr    Address;
};

struct MLME_DISASSOCIATE_indication_pset {
	uint8_t            DevAddr[8];
	uint8_t            Reason;
	struct SecSpec     Security;
};


struct MLME_BEACON_NOTIFY_indication_pset {
	uint8_t              BSN;
	struct PanDescriptor PanDescriptor;    /* variable size and so following
                                         fields have to be dealt with
                                         separately */
	/* uint8_t         PendAddrSpec; */
	/* variable        Address List  */
	/* variable        Beacon payload */
};

struct MLME_GET_confirm_pset {
	uint8_t            Status;
	uint8_t            PIBAttribute;
	uint8_t            PIBAttributeIndex;
	uint8_t            PIBAttributeLength;
	uint8_t            PIBAttributeValue[MAX_ATTRIBUTE_SIZE];
};

#define MLME_GET_CONFIRM_BASE_SIZE  (sizeof(struct MLME_GET_confirm_pset)-MAX_ATTRIBUTE_SIZE)

#define DEFAULT_RESULT_LIST_SIZE    (16)
struct MLME_SCAN_confirm_pset {
	uint8_t            Status;
	uint8_t            ScanType;
	uint8_t            UnscannedChannels[4];
	uint8_t            ResultListSize;
	uint8_t            ResultList[DEFAULT_RESULT_LIST_SIZE];
};

struct MLME_COMM_STATUS_indication_pset {
	uint8_t            PANId[2];
	uint8_t            SrcAddrMode;
	uint8_t            SrcAddr[8];
	uint8_t            DstAddrMode;
	uint8_t            DstAddr[8];
	uint8_t            Status;
	struct SecSpec     Security;
};

struct MLME_ORPHAN_indication_pset {
	uint8_t            OrphanAddr[8];
	struct SecSpec     Security;
};

struct MLME_SYNC_LOSS_indication_pset {
	uint8_t            LossReason;
	uint8_t            PANId[2];
	uint8_t            LogicalChannel;
	struct SecSpec     Security;
};

// HWME

struct HWME_SET_confirm_pset {
	uint8_t            Status;
	uint8_t            HWAttribute;
};

struct HWME_GET_confirm_pset {
	uint8_t            Status;
	uint8_t            HWAttribute;
	uint8_t            HWAttributeLength;
	uint8_t            HWAttributeValue[MAX_HWME_ATTRIBUTE_SIZE];
};

struct HWME_HAES_confirm_pset {
	uint8_t            Status;
	uint8_t            HAESData[16];
};

struct HWME_WAKEUP_indication_pset {
	uint8_t            WakeUpCondition;
};

// TDME

struct TDME_SETSFR_confirm_pset {
	uint8_t            Status;
	uint8_t            SFRPage;
	uint8_t            SFRAddress;
};

struct TDME_GETSFR_confirm_pset {
	uint8_t            Status;
	uint8_t            SFRPage;
	uint8_t            SFRAddress;
	uint8_t            SFRValue;
};

struct TDME_TESTMODE_confirm_pset {
	uint8_t            Status;
	uint8_t            TestMode;
};

struct TDME_SET_confirm_pset {
	uint8_t            Status;
	uint8_t            TDAttribute;
};

struct TDME_TXPKT_confirm_pset {
	uint8_t            Status;
	uint8_t            TestPacketSequenceNumber;
	uint8_t            TestPacketLength;
	uint8_t            TestPacketData[128];
};

struct TDME_RXPKT_indication_pset {
	uint8_t            Status;
	uint8_t            TestPacketEDValue;
	uint8_t            TestPacketCSValue;
	uint8_t            TestPacketFoffsValue;
	uint8_t            TestPacketLength;
	uint8_t            TestPacketData[128];
};

struct TDME_EDDET_indication_pset {
	uint8_t            TestEDThreshold;
	uint8_t            TestEDValue;
	uint8_t            TestCSValue;
	uint8_t            TestTimeAboveThreshold_us[2];
};

struct TDME_ERROR_indication_pset {
	uint8_t            ErrorCode;
};

struct TDME_LOTLK_confirm_pset {
	uint8_t            Status;
	uint8_t            TestChannel;
	uint8_t            TestRxTxb;
	uint8_t            TestLOFDACValue;
	uint8_t            TestLOAMPValue;
	uint8_t            TestLOTXCALValue;
};


/******************************************************************************/
/****** Security PIB Table Size Definitions                              ******/
/******************************************************************************/
#define KEY_TABLE_SIZE                  (2)
#define LOOKUP_DESC_TABLE_SIZE          (5)
#define KEY_DEVICE_TABLE_SIZE           (5)
#define KEY_USAGE_TABLE_SIZE            (12)
#define SECURITY_LEVEL_TABLE_SIZE       (2)
#define DEVICE_TABLE_SIZE               (5)


/******************************************************************************/
/****** Structures used by Security PIB Attributes                       ******/
/******************************************************************************/

struct M_KeyIdLookupDesc {
    uint8_t      LookupData[9];
    uint8_t      LookupDataSizeCode;
};

struct M_DeviceDescriptor {
	uint8_t      PANId[2];
	uint8_t      ShortAddress[2];
	uint8_t      ExtAddress[8];
	uint8_t      FrameCounter[4];
	uint8_t      Exempt;
};

struct M_SecurityLevelDescriptor {
	uint8_t    FrameType;
	uint8_t    CommandFrameIdentifier;
	uint8_t    SecurityMinimum;
	uint8_t    DeviceOverrideSecurityMinimum;
};

struct M_KeyDeviceDesc {
	// uint8_t      DeviceDescriptorHandle : 6;
	// uint8_t      UniqueDevice : 1;
	// uint8_t      Blacklisted : 1;
	uint8_t      Flags;

};
/* Masks for KeyDeviceDesc Flags*/
#define KDD_DeviceDescHandleMask        (0x3F)
#define KDD_UniqueDeviceMask            (0x40)
#define KDD_BlacklistedMask             (0x80)

struct M_KeyUsageDesc {
	// uint8_t      FrameType : 2;
	// uint8_t      CommandFrameIdentifier : 4;
	uint8_t      Flags;
};
/* Masks for KeyUsageDesc Flags*/
#define KUD_FrameTypeMask               (0x03)
#define KUD_CommandFrameIdentifierMask  (0xF0)
#define KUD_CommandFrameIdentifierShift (4)

struct M_KeyTableEntryFixed {
	uint8_t    KeyIdLookupListEntries;
	uint8_t    KeyDeviceListEntries;
	uint8_t    KeyUsageListEntries;
	uint8_t    Key[16];
};

struct M_KeyDescriptor {
	struct M_KeyTableEntryFixed    Fixed;
	struct M_KeyIdLookupDesc       KeyIdLookupList[LOOKUP_DESC_TABLE_SIZE];
	struct M_KeyDeviceDesc         KeyDeviceList[KEY_DEVICE_TABLE_SIZE];
	struct M_KeyUsageDesc          KeyUsageList[SECURITY_LEVEL_TABLE_SIZE];
};


/******************************************************************************/
/****** Message ID Codes in SPI Commands                                 ******/
/******************************************************************************/
// Downstream
#define MCPS_DATA_REQUEST                     (0x00)
#define MCPS_PURGE_REQUEST                    (0x01)
#define MLME_ASSOCIATE_REQUEST                (0x02)
#define MLME_ASSOCIATE_RESPONSE               (0x03)
#define MLME_DISASSOCIATE_REQUEST             (0x04)
#define MLME_GET_REQUEST                      (0x05)
#define MLME_ORPHAN_RESPONSE                  (0x06)
#define MLME_RESET_REQUEST                    (0x07)
#define MLME_RX_ENABLE_REQUEST                (0x08)
#define MLME_SCAN_REQUEST                     (0x09)
#define MLME_SET_REQUEST                      (0x0A)
#define MLME_START_REQUEST                    (0x0B)
#define MLME_SYNC_REQUEST                     (0x0C)
#define MLME_POLL_REQUEST                     (0x0D)
#define HWME_SET_REQUEST                      (0x0E)
#define HWME_GET_REQUEST                      (0x0F)
#define HWME_HAES_REQUEST                     (0x10)
#define TDME_SETSFR_REQUEST                   (0x11)
#define TDME_GETSFR_REQUEST                   (0x12)
#define TDME_TESTMODE_REQUEST                 (0x13)
#define TDME_SET_REQUEST                      (0x14)
#define TDME_TXPKT_REQUEST                    (0x15)
#define TDME_LOTLK_REQUEST                    (0x16)
// Upstream
#define MCPS_DATA_INDICATION                  (0x00)
#define MCPS_DATA_CONFIRM                     (0x01)
#define MCPS_PURGE_CONFIRM                    (0x02)
#define MLME_ASSOCIATE_INDICATION             (0x03)
#define MLME_ASSOCIATE_CONFIRM                (0x04)
#define MLME_DISASSOCIATE_INDICATION          (0x05)
#define MLME_DISASSOCIATE_CONFIRM             (0x06)
#define MLME_BEACON_NOTIFY_INDICATION         (0x07)
#define MLME_GET_CONFIRM                      (0x08)
#define MLME_ORPHAN_INDICATION                (0x09)
#define MLME_RESET_CONFIRM                    (0x0A)
#define MLME_RX_ENABLE_CONFIRM                (0x0B)
#define MLME_SCAN_CONFIRM                     (0x0C)
#define MLME_COMM_STATUS_INDICATION           (0x0D)
#define MLME_SET_CONFIRM                      (0x0E)
#define MLME_START_CONFIRM                    (0x0F)
#define MLME_SYNC_LOSS_INDICATION             (0x10)
#define MLME_POLL_CONFIRM                     (0x11)
#define HWME_SET_CONFIRM                      (0x12)
#define HWME_GET_CONFIRM                      (0x13)
#define HWME_HAES_CONFIRM                     (0x14)
#define HWME_WAKEUP_INDICATION                (0x15)
#define TDME_MESSAGE_INDICATION               (0x16)
#define TDME_SETSFR_CONFIRM                   (0x17)
#define TDME_GETSFR_CONFIRM                   (0x18)
#define TDME_TESTMODE_CONFIRM                 (0x19)
#define TDME_SET_CONFIRM                      (0x1A)
#define TDME_TXPKT_CONFIRM                    (0x1B)
#define TDME_RXPKT_INDICATION                 (0x1C)
#define TDME_EDDET_INDICATION                 (0x1D)
#define TDME_ERROR_INDICATION                 (0x1E)
#define TDME_LOTLK_CONFIRM                    (0x1F)


/******************************************************************************/
/****** SPI Message Format Typedef                                       ******/
/******************************************************************************/

struct MAC_Message {
	uint8_t      CommandId;
	uint8_t      Length;
	union {
		/* MAC MCPS / MLME */
		struct MCPS_DATA_request_pset               DataReq;
		struct MLME_ASSOCIATE_request_pset          AssocReq;
		struct MLME_ASSOCIATE_response_pset         AssocRsp;
		struct MLME_DISASSOCIATE_request_pset       DisassocReq;
		struct MLME_GET_request_pset                GetReq;
		struct MLME_ORPHAN_response_pset            OrphanRsp;
		struct MLME_POLL_request_pset               PollReq;
		struct MLME_RX_ENABLE_request_pset          RxEnableReq;
		struct MLME_SCAN_request_pset               ScanReq;
		struct MLME_SET_request_pset                SetReq;
		struct MLME_START_request_pset              StartReq;
		struct MCPS_DATA_confirm_pset               DataCnf;
		struct MCPS_PURGE_confirm_pset              PurgeCnf;
		struct MCPS_DATA_indication_pset            DataInd;
		struct MLME_ASSOCIATE_indication_pset       AssocInd;
		struct MLME_ASSOCIATE_confirm_pset          AssocCnf;
		struct MLME_DISASSOCIATE_indication_pset    DisassocInd;
		struct MLME_DISASSOCIATE_confirm_pset       DisassocCnf;
		struct MLME_BEACON_NOTIFY_indication_pset   BeaconInd;
		struct MLME_GET_confirm_pset                GetCnf;
		struct MLME_SCAN_confirm_pset               ScanCnf;
		struct MLME_COMM_STATUS_indication_pset     CommStatusInd;
		struct MLME_SYNC_LOSS_indication_pset       SyncLossInd;
		struct MLME_ORPHAN_indication_pset          OrphanInd;
		/* HWME */
		struct HWME_SET_request_pset                HWMESetReq;
		struct HWME_GET_request_pset                HWMEGetReq;
		struct HWME_HAES_request_pset               HWMEHAESReq;
		struct HWME_SET_confirm_pset                HWMESetCnf;
		struct HWME_GET_confirm_pset                HWMEGetCnf;
		struct HWME_HAES_confirm_pset               HWMEHAESCnf;
		struct HWME_WAKEUP_indication_pset          HWMEWakeupInd;
		/* TDME */
		struct TDME_SETSFR_request_pset             TDMESetSFRReq;
		struct TDME_GETSFR_request_pset             TDMEGetSFRReq;
		struct TDME_TESTMODE_request_pset             TDMETestModeReq;
		struct TDME_SET_request_pset                TDMESetReq;
		struct TDME_TXPKT_request_pset                TDMETxPktReq;
		struct TDME_LOTLK_request_pset              TDMELOTlkReq;
		struct TDME_SETSFR_confirm_pset             TDMESetSFRCnf;
		struct TDME_GETSFR_confirm_pset             TDMEGetSFRCnf;
		struct TDME_TESTMODE_confirm_pset             TDMETestModeCnf;
		struct TDME_SET_confirm_pset                TDMESetCnf;
		struct TDME_TXPKT_confirm_pset                TDMETxPktCnf;
		struct TDME_RXPKT_indication_pset           TDMERxPktInd;
		struct TDME_EDDET_indication_pset           TDMEEDDetInd;
		struct TDME_ERROR_indication_pset           TDMEErrorInd;
		struct TDME_LOTLK_confirm_pset              TDMELOTlkCnf;
		/* Common */
		uint8_t                                     u8Param;
		uint8_t                                     Status;
		uint8_t                                     Payload[254];
	} PData;
};


/******************************************************************************/
/****** SPI Command IDs                                                  ******/
/******************************************************************************/
// Mask to derive the Message ID Code from the Command ID
#define SPI_MID_MASK                          (0x1F)
// Bit indicating a Confirm or Indication from Slave to Master
#define SPI_S2M                               (0x20)
// Bit indicating a Synchronous Message
#define SPI_SYN                               (0x40)


/******************************************************************************/
/****** SPI Command Definitions                                          ******/
/******************************************************************************/
// Idle Mode - No Data
#define SPI_IDLE                           (0xFF)
// NACK - Buffer full or busy - resend Request
#define SPI_NACK                           (0xF0)
// MAC MCPS
#define SPI_MCPS_DATA_REQUEST              (MCPS_DATA_REQUEST)
#define SPI_MCPS_PURGE_REQUEST             (MCPS_PURGE_REQUEST+SPI_SYN)
#define SPI_MCPS_DATA_INDICATION           (MCPS_DATA_INDICATION+SPI_S2M)
#define SPI_MCPS_DATA_CONFIRM              (MCPS_DATA_CONFIRM+SPI_S2M)
#define SPI_MCPS_PURGE_CONFIRM             (MCPS_PURGE_CONFIRM+SPI_S2M+SPI_SYN)
// MAC MLME
#define SPI_MLME_ASSOCIATE_REQUEST         (MLME_ASSOCIATE_REQUEST)
#define SPI_MLME_ASSOCIATE_RESPONSE        (MLME_ASSOCIATE_RESPONSE)
#define SPI_MLME_DISASSOCIATE_REQUEST      (MLME_DISASSOCIATE_REQUEST)
#define SPI_MLME_GET_REQUEST               (MLME_GET_REQUEST+SPI_SYN)
#define SPI_MLME_ORPHAN_RESPONSE           (MLME_ORPHAN_RESPONSE)
#define SPI_MLME_RESET_REQUEST             (MLME_RESET_REQUEST+SPI_SYN)
#define SPI_MLME_RX_ENABLE_REQUEST         (MLME_RX_ENABLE_REQUEST+SPI_SYN)
#define SPI_MLME_SCAN_REQUEST              (MLME_SCAN_REQUEST)
#define SPI_MLME_SET_REQUEST               (MLME_SET_REQUEST+SPI_SYN)
#define SPI_MLME_START_REQUEST             (MLME_START_REQUEST+SPI_SYN)
#define SPI_MLME_SYNC_REQUEST              (MLME_SYNC_REQUEST)
#define SPI_MLME_POLL_REQUEST              (MLME_POLL_REQUEST+SPI_SYN)
#define SPI_MLME_ASSOCIATE_INDICATION      (MLME_ASSOCIATE_INDICATION+SPI_S2M)
#define SPI_MLME_ASSOCIATE_CONFIRM         (MLME_ASSOCIATE_CONFIRM+SPI_S2M)
#define SPI_MLME_DISASSOCIATE_INDICATION   (MLME_DISASSOCIATE_INDICATION+SPI_S2M)
#define SPI_MLME_DISASSOCIATE_CONFIRM      (MLME_DISASSOCIATE_CONFIRM+SPI_S2M)
#define SPI_MLME_BEACON_NOTIFY_INDICATION  (MLME_BEACON_NOTIFY_INDICATION+SPI_S2M)
#define SPI_MLME_GET_CONFIRM               (MLME_GET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_ORPHAN_INDICATION         (MLME_ORPHAN_INDICATION+SPI_S2M)
#define SPI_MLME_RESET_CONFIRM             (MLME_RESET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_RX_ENABLE_CONFIRM         (MLME_RX_ENABLE_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_SCAN_CONFIRM              (MLME_SCAN_CONFIRM+SPI_S2M)
#define SPI_MLME_COMM_STATUS_INDICATION    (MLME_COMM_STATUS_INDICATION+SPI_S2M)
#define SPI_MLME_SET_CONFIRM               (MLME_SET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_START_CONFIRM             (MLME_START_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_MLME_SYNC_LOSS_INDICATION      (MLME_SYNC_LOSS_INDICATION+SPI_S2M)
#define SPI_MLME_POLL_CONFIRM              (MLME_POLL_CONFIRM+SPI_S2M+SPI_SYN)
// HWME
#define SPI_HWME_SET_REQUEST               (HWME_SET_REQUEST+SPI_SYN)
#define SPI_HWME_GET_REQUEST               (HWME_GET_REQUEST+SPI_SYN)
#define SPI_HWME_HAES_REQUEST              (HWME_HAES_REQUEST+SPI_SYN)
#define SPI_HWME_SET_CONFIRM               (HWME_SET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_HWME_GET_CONFIRM               (HWME_GET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_HWME_HAES_CONFIRM              (HWME_HAES_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_HWME_WAKEUP_INDICATION         (HWME_WAKEUP_INDICATION+SPI_S2M)
// TDME
#define SPI_TDME_SETSFR_REQUEST            (TDME_SETSFR_REQUEST+SPI_SYN)
#define SPI_TDME_GETSFR_REQUEST            (TDME_GETSFR_REQUEST+SPI_SYN)
#define SPI_TDME_TESTMODE_REQUEST          (TDME_TESTMODE_REQUEST+SPI_SYN)
#define SPI_TDME_SET_REQUEST               (TDME_SET_REQUEST+SPI_SYN)
#define SPI_TDME_TXPKT_REQUEST             (TDME_TXPKT_REQUEST+SPI_SYN)
#define SPI_TDME_LOTLK_REQUEST             (TDME_LOTLK_REQUEST+SPI_SYN)
#define SPI_TDME_MESSAGE_INDICATION        (TDME_MESSAGE_INDICATION+SPI_S2M)
#define SPI_TDME_SETSFR_CONFIRM            (TDME_SETSFR_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_GETSFR_CONFIRM            (TDME_GETSFR_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_TESTMODE_CONFIRM          (TDME_TESTMODE_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_SET_CONFIRM               (TDME_SET_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_TXPKT_CONFIRM             (TDME_TXPKT_CONFIRM+SPI_S2M+SPI_SYN)
#define SPI_TDME_RXPKT_INDICATION          (TDME_RXPKT_INDICATION+SPI_S2M)
#define SPI_TDME_EDDET_INDICATION          (TDME_EDDET_INDICATION+SPI_S2M)
#define SPI_TDME_ERROR_INDICATION          (TDME_ERROR_INDICATION+SPI_S2M)
#define SPI_TDME_LOTLK_CONFIRM             (TDME_LOTLK_CONFIRM+SPI_S2M+SPI_SYN)


#endif  // MAC_MESSAGES_H
