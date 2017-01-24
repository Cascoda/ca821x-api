/**
 * @file ca821x_api.h
 * @brief API Access Function Declarations for MCPS, MLME, HWME and TDME.
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
/***************************************************************************//**
 * \def LS_BYTE(x)
 * Extract the least significant octet of a 16-bit value
 * \def MS_BYTE(x)
 * Extract the most significant octet of a 16-bit value
 * \def LS0_BYTE(x)
 * Extract the first (little-endian) octet of a 32-bit value
 * \def LS1_BYTE(x)
 * Extract the second (little-endian) octet of a 32-bit value
 * \def LS2_BYTE(x)
 * Extract the third (little-endian) octet of a 32-bit value
 * \def LS3_BYTE(x)
 * Extract the fourth (little-endian) octet of a 32-bit value
 * \def GETLE16(x)
 * Extract a 16-bit value from a little-endian octet array
 * \def GETLE32(x)
 * Extract a 32-bit value from a little-endian octet array
 * \def PUTLE16(x,y)
 * Put a 16-bit value x into a little-endian octet array y
 * \def PUTLE32(x,y)
 * Put a 32-bit value x into a little-endian octet array y
 ******************************************************************************/
#ifndef CA821X_API_H
#define CA821X_API_H

#include <stddef.h>
#include <stdint.h>

#include "mac_messages.h"

#ifndef _CASCODA_MACROS
#define _CASCODA_MACROS

#define LS_BYTE(x)  ((uint8_t)((x)&0xFF))
#define MS_BYTE(x)  ((uint8_t)(((x)>>8)&0xFF))
#define LS0_BYTE(x) ((uint8_t)((x)&0xFF))
#define LS1_BYTE(x) ((uint8_t)(((x)>>8)&0xFF))
#define LS2_BYTE(x) ((uint8_t)(((x)>>16)&0xFF))
#define LS3_BYTE(x) ((uint8_t)(((x)>>24)&0xFF))

#define GETLE16(x)      (((uint16_t)(x)[1]<<8)+(x)[0])
#define GETLE32(x)      (((uint32_t)(x)[3]<<24)+((uint32_t)(x)[2]<<16)+((uint32_t)(x)[1]<<8)+(x)[0])
#define PUTLE16(x,y)    {(y)[0]=((x)&0xff);(y)[1]=((x)>>8);}
#define PUTLE32(x,y)    {(y)[0]=((x)&0xff);(y)[1]=(((x)>>8)&0xff);(y)[2]=(((x)>>16)&0xff);(y)[3]=(((x)>>24)&0xff);}

#endif

/***************************************************************************//**
 * \brief API user callbacks structure
 *
 * Contains a set of function pointers that can (and should) be populated by the
 * user for processing asynchronous messages received from the hardware. If the
 * pointer for the specific command type is populated that will be called,
 * otherwise the generic_dispatch function will be. If neither are populated the
 * message is discarded.
 ******************************************************************************/
struct ca821x_api_callbacks {
	int (*MCPS_DATA_indication) (
		struct MCPS_DATA_indication_pset *params);
	int (*MCPS_DATA_confirm) (
		struct MCPS_DATA_confirm_pset *params);
	int (*MLME_ASSOCIATE_indication) (
		struct MLME_ASSOCIATE_indication_pset *params);
	int (*MLME_ASSOCIATE_confirm) (
		struct MLME_ASSOCIATE_confirm_pset *params);
	int (*MLME_DISASSOCIATE_indication) (
		struct MLME_DISASSOCIATE_indication_pset *params);
	int (*MLME_DISASSOCIATE_confirm) (
		struct MLME_DISASSOCIATE_confirm_pset *params);
	int (*MLME_BEACON_NOTIFY_indication) (
		struct MLME_BEACON_NOTIFY_indication_pset *params);
	int (*MLME_ORPHAN_indication) (
		struct MLME_ORPHAN_indication_pset *params);
	int (*MLME_SCAN_confirm) (
		struct MLME_SCAN_confirm_pset *params);
	int (*MLME_COMM_STATUS_indication) (
		struct MLME_COMM_STATUS_indication_pset *params);
	int (*MLME_SYNC_LOSS_indication) (
		struct MLME_SYNC_LOSS_indication_pset *params);
	int (*HWME_WAKEUP_indication) (
		struct HWME_WAKEUP_indication_pset *params);
	int (*TDME_MESSAGE_indication) (
		const char *message,
		size_t      len);
	int (*TDME_RXPKT_indication) (
		struct TDME_RXPKT_indication_pset *params);
	int (*TDME_EDDET_indication) (
		struct TDME_EDDET_indication_pset *params);
	int (*TDME_ERROR_indication) (
		struct TDME_ERROR_indication_pset *params);
	int (*generic_dispatch) (
		const uint8_t *buf,
		size_t         len);
};

/******************************************************************************/
/****** MAC MCPS/MLME Downlink                                           ******/
/******************************************************************************/

uint8_t MCPS_DATA_request(
	uint8_t          SrcAddrMode,
	uint8_t          DstAddrMode,
	uint16_t         DstPANId,
	union MacAddr   *pDstAddr,
	uint8_t          MsduLength,
	uint8_t         *pMsdu,
	uint8_t          MsduHandle,
	uint8_t          TxOptions,
	struct SecSpec  *pSecurity,
	void            *pDeviceRef
);

uint8_t MCPS_PURGE_request_sync(
	uint8_t     *MsduHandle,
	void        *pDeviceRef
);

uint8_t MLME_ASSOCIATE_request(
	uint8_t          LogicalChannel,
	uint8_t          DstAddrMode,
	uint16_t         DstPANId,
	union MacAddr   *pDstAddr,
	uint8_t          CapabilityInfo,
	struct SecSpec  *pSecurity,
	void            *pDeviceRef
);

uint8_t MLME_ASSOCIATE_response(
	uint8_t         *pDeviceAddress,
	uint16_t         AssocShortAddress,
	uint8_t          Status,
	struct SecSpec  *pSecurity,
	void            *pDeviceRef
);

uint8_t MLME_DISASSOCIATE_request(
	struct FullAddr    DevAddr,
	uint8_t            DisassociateReason,
	uint8_t            TxIndirect,
	struct SecSpec    *pSecurity,
	void              *pDeviceRef
);

uint8_t MLME_GET_request_sync(
	uint8_t      PIBAttribute,
	uint8_t      PIBAttributeIndex,
	uint8_t     *pPIBAttributeLength,
	void        *pPIBAttributeValue,
	void        *pDeviceRef
);

uint8_t MLME_ORPHAN_response(
	uint8_t          *pOrphanAddress,
	uint16_t          ShortAddress,
	uint8_t           AssociatedMember,
	struct SecSpec   *pSecurity,
	void             *pDeviceRef
);

uint8_t MLME_RESET_request_sync(uint8_t SetDefaultPIB, void *pDeviceRef);

uint8_t MLME_RX_ENABLE_request_sync(
	uint8_t      DeferPermit,
	uint32_t     RxOnTime,
	uint32_t     RxOnDuration,
	void        *pDeviceRef
);

uint8_t MLME_SCAN_request(
	uint8_t          ScanType,
	uint32_t         ScanChannels,
	uint8_t          ScanDuration,
	struct SecSpec  *pSecurity,
	void            *pDeviceRef
);

uint8_t MLME_SET_request_sync(
	uint8_t       PIBAttribute,
	uint8_t       PIBAttributeIndex,
	uint8_t       PIBAttributeLength,
	const void   *pPIBAttributeValue,
	void         *pDeviceRef
);

uint8_t MLME_START_request_sync(
	uint16_t          PANId,
	uint8_t           LogicalChannel,
	uint8_t           BeaconOrder,
	uint8_t           SuperframeOrder,
	uint8_t           PANCoordinator,
	uint8_t           BatteryLifeExtension,
	uint8_t           CoordRealignment,
	struct SecSpec   *pCoordRealignSecurity,
	struct SecSpec   *pBeaconSecurity,
	void             *pDeviceRef
);

uint8_t MLME_POLL_request_sync(
	struct FullAddr   CoordAddress,
	uint8_t           Interval[2],    /* polling interval in 0.1 seconds res */
	                                  /* 0 means poll once */
	                                  /* 0xFFFF means stop polling */
	struct SecSpec   *pSecurity,
	void             *pDeviceRef
);

/******************************************************************************/
/****** HWME Downlink                                                    ******/
/******************************************************************************/

uint8_t HWME_SET_request_sync(
	uint8_t      HWAttribute,
	uint8_t      HWAttributeLength,
	uint8_t     *pHWAttributeValue,
	void        *pDeviceRef
);

uint8_t HWME_GET_request_sync(
	uint8_t      HWAttribute,
	uint8_t     *HWAttributeLength,
	uint8_t     *pHWAttributeValue,
	void        *pDeviceRef
);

uint8_t HWME_HAES_request_sync(
	uint8_t      HAESMode,
	uint8_t     *pHAESData,
	void        *pDeviceRef
);

/******************************************************************************/
/****** TDME Downlink                                                    ******/
/******************************************************************************/

uint8_t TDME_SETSFR_request_sync(
	uint8_t      SFRPage,
	uint8_t      SFRAddress,
	uint8_t      SFRValue,
	void         *pDeviceRef
);

uint8_t TDME_GETSFR_request_sync(
	uint8_t      SFRPage,
	uint8_t      SFRAddress,
	uint8_t     *SFRValue,
	void        *pDeviceRef
);

uint8_t TDME_TESTMODE_request_sync(
	uint8_t      TestMode,
	void        *pDeviceRef
);

uint8_t TDME_SET_request_sync(
	uint8_t      TestAttribute,
	uint8_t      TestAttributeLength,
	void        *pTestAttributeValue,
	void        *pDeviceRef
);

uint8_t TDME_TXPKT_request_sync(
	uint8_t      TestPacketDataType,
	uint8_t     *TestPacketSequenceNumber,
	uint8_t     *TestPacketLength,
	void        *pTestPacketData,
	void        *pDeviceRef
);

uint8_t TDME_LOTLK_request_sync(
	uint8_t     *TestChannel,
	uint8_t     *TestRxTxb,
	uint8_t     *TestLOFDACValue,
	uint8_t     *TestLOAMPValue,
	uint8_t     *TestLOTXCALValue,
	void        *pDeviceRef
);

/******************************************************************************/
/****** TDME Register Default Initialisation and Checking Functions      ******/
/******************************************************************************/
uint8_t TDME_ChipInit(void *pDeviceRef);
uint8_t TDME_ChannelInit(uint8_t channel, void *pDeviceRef);
uint8_t TDME_CheckPIBAttribute(
	uint8_t      PIBAttribute,
	uint8_t      PIBAttributeLength,
	const void   *pPIBAttributeValue
);

uint8_t TDME_SetTxPower(uint8_t txp, void *pDeviceRef);
uint8_t TDME_GetTxPower(uint8_t *txp, void *pDeviceRef);

/******************************************************************************/
/****** API callback functions                                           ******/
/******************************************************************************/

void ca821x_register_callbacks(struct ca821x_api_callbacks *in_callbacks);
int ca821x_downstream_dispatch(const uint8_t *buf, size_t len);

/******************************************************************************/
/****** External function pointers                                       ******/
/******************************************************************************/
extern int (*ca821x_api_downstream)(
	const uint8_t *buf,
	size_t len,
	uint8_t *response,
	void *pDeviceRef
);

/******************************************************************************/
/****** MAC Workarounds for V1.1 and MPW silicon (V0.x)                  ******/
/******************************************************************************/
extern uint8_t MAC_Workarounds;
extern uint8_t MAC_MPW;


#endif // CA821X_API_H
