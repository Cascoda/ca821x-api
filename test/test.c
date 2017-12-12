#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ca821x_api.h"

#define TEST_DSTADDR 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
#define TEST_CHANNEL (13)
#define TEST_PANID 0x5C, 0xCA

#define TEST_MSDULENGTH (4)
#define TEST_MSDUHANDLE (0xAA)
#define TEST_MSDU 0xDE, 0xAD, 0xBE, 0xEF

#define TEST_SECURITYLEVEL (7)
#define TEST_KEYIDMODE (3)
#define TEST_KEYSOURCE 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00
#define TEST_KEYINDEX (0x01)

#define TEST_PIBATTRIBUTE (phyCurrentChannel)
#define TEST_PIBATTRIBUTEVALUE (TEST_CHANNEL)
#define TEST_HWATTRIBUTE (HWME_EDTHRESHOLD)
#define TEST_HWATTRIBUTEVALUE (100)
#define TEST_TDMEATTRIBUTE (TDME_CHANNEL)
#define TEST_TDMEATTRIBUTEVALUE (TEST_CHANNEL)

#define TEST_HAESMODE (0)
#define TEST_HAESDATA 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, \
                      0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff

#define TEST_SFRPAGE (1)
#define TEST_SFRADDRESS (0xBF)
#define TEST_SFRVALUE (0xAE)

#define TEST_TESTMODE (TDME_TEST_IDLE)

#define TEST_SEQUENCENUM (0)

#define TEST_DATAREQLEN (26+TEST_MSDULENGTH)

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

uint8_t ref_mcps_data_req[] = {
	SPI_MCPS_DATA_REQUEST, /* CmdId */
	TEST_DATAREQLEN, /* Packet Length */
	MAC_MODE_SHORT_ADDR, /* SrcAddrMode */
	MAC_MODE_LONG_ADDR, /* DstAddrMode */
	TEST_PANID, /* DstPANId */
	TEST_DSTADDR, /* DstAddr */
	TEST_MSDULENGTH, /* MsduLength */
	TEST_MSDUHANDLE, /* MsduHandle */
	0x00, /* TxOptions */
	TEST_MSDU, /* Msdu */
	TEST_SECURITYLEVEL, /* SecurityLevel */
	TEST_KEYIDMODE, /* KeyIdMode */
	TEST_KEYSOURCE, /* KeySource */
	TEST_KEYINDEX /* KeyIndex */
};

uint8_t ref_mcps_purge_req[] = {
	SPI_MCPS_PURGE_REQUEST, /* CmdId */
	1, /* Packet Length */
	TEST_MSDUHANDLE, /* MsduHandle */
};

uint8_t ref_mlme_associate_req[] = {
	SPI_MLME_ASSOCIATE_REQUEST, /* CmdId */
	24, /* Packet Length */
	TEST_CHANNEL, /* LogicalChannel */
	MAC_MODE_LONG_ADDR, /* CoordAddrMode */
	TEST_PANID, /* CoordPANId */
	TEST_DSTADDR, /* CoordAddress */
	0x00, /* CapabilityInformation */
	TEST_SECURITYLEVEL, /* SecurityLevel */
	TEST_KEYIDMODE, /* KeyIdMode */
	TEST_KEYSOURCE, /* KeySource */
	TEST_KEYINDEX /* KeyIndex */
};

uint8_t ref_mlme_associate_resp[] = {
	SPI_MLME_ASSOCIATE_RESPONSE, /* CmdId */
	22, /* Packet Length */
	TEST_DSTADDR, /* DeviceAddress */
	0x01, 0xCA, /* AssocShortAddress */
	0x00, /* Status */
	TEST_SECURITYLEVEL, /* SecurityLevel */
	TEST_KEYIDMODE, /* KeyIdMode */
	TEST_KEYSOURCE, /* KeySource */
	TEST_KEYINDEX /* KeyIndex */
};

uint8_t ref_mlme_disassociate_req[] = {
	SPI_MLME_DISASSOCIATE_REQUEST, /* CmdId */
	24, /* Packet Length */
	MAC_MODE_LONG_ADDR, /* DeviceAddrMode */
	TEST_PANID, /* DevicePANId */
	TEST_DSTADDR, /* DeviceAddress */
	0x00, /* DisassociateReason */
	0, /* TxIndirect */
	TEST_SECURITYLEVEL, /* SecurityLevel */
	TEST_KEYIDMODE, /* KeyIdMode */
	TEST_KEYSOURCE, /* KeySource */
	TEST_KEYINDEX /* KeyIndex */
};

uint8_t ref_mlme_get_req[] = {
	SPI_MLME_GET_REQUEST, /* CmdId */
	2, /* Packet Length */
	TEST_PIBATTRIBUTE, /* PIBAttribute */
	0x00 /* PIBAttributeIndex */
};

uint8_t ref_mlme_orphan_resp[] = {
	SPI_MLME_ORPHAN_RESPONSE, /* CmdId */
	22, /* Packet Length */
	TEST_DSTADDR, /* OrphanAddress */
	0x01, 0xCA, /* ShortAddress */
	0, /* AssociatedMember */
	TEST_SECURITYLEVEL, /* SecurityLevel */
	TEST_KEYIDMODE, /* KeyIdMode */
	TEST_KEYSOURCE, /* KeySource */
	TEST_KEYINDEX /* KeyIndex */
};

uint8_t ref_mlme_reset_req[] = {
	SPI_MLME_RESET_REQUEST, /* CmdId */
	1, /* Packet Length */
	0 /* SetDefaultPIB */
};

uint8_t ref_mlme_rx_enable_req[] = {
	SPI_MLME_RX_ENABLE_REQUEST, /* CmdId */
	9, /* Packet Length */
	0x00, /* DeferPermit */
	0x0A, 0x00, 0x00, 0xA0, /* RxOnTime */
	0x0B, 0x00, 0x00, 0xB0 /* RxOnDuration */
};

uint8_t ref_mlme_scan_req[] = {
	SPI_MLME_SCAN_REQUEST, /* CmdId */
	17, /* Packet Length */
	ACTIVE_SCAN, /* ScanType */
	0x00, 0xF8, 0xFF, 0x07, /* ScanChannels */
	0x01, /* ScanDuration */
	TEST_SECURITYLEVEL, /* SecurityLevel */
	TEST_KEYIDMODE, /* KeyIdMode */
	TEST_KEYSOURCE, /* KeySource */
	TEST_KEYINDEX /* KeyIndex */
};

uint8_t ref_mlme_set_req[] = {
	SPI_MLME_SET_REQUEST, /* CmdId */
	4, /* Packet Length */
	TEST_PIBATTRIBUTE, /* PIBAttribute */
	0x00, /* PIBAttributeIndex */
	0x01, /* PIBAttributeLength */
	TEST_PIBATTRIBUTEVALUE
};

uint8_t ref_mlme_start_req[] = {
	SPI_MLME_START_REQUEST, /* CmdId */
	30, /* Packet Length */
	TEST_PANID, /* PANId */
	TEST_CHANNEL,
	0x0F, /* BeaconOrder */
	0x0F, /* SuperframeOrder */
	0, /* PANCoordinator */
	0, /* BatteryLifeExtension */
	0, /* CoordRealignment */
	TEST_SECURITYLEVEL, /* CoordRealignSecurityLevel */
	TEST_KEYIDMODE, /* CoordRealignKeyIdMode */
	TEST_KEYSOURCE, /* CoordRealignKeySource */
	TEST_KEYINDEX, /* CoordRealignKeyIndex */
	TEST_SECURITYLEVEL, /* BeaconSecurityLevel */
	TEST_KEYIDMODE, /* BeaconKeyIdMode */
	TEST_KEYSOURCE, /* BeaconKeySource */
	TEST_KEYINDEX, /* BeaconKeyIndex */
};

uint8_t ref_mlme_poll_req[] = {
	SPI_MLME_POLL_REQUEST, /* CmdId */
	24, /* Packet Length */
	MAC_MODE_LONG_ADDR, /* CoordAddressMode */
	TEST_PANID, /* CoordinatorPANId */
	TEST_DSTADDR, /* CoordAddress */
	0x00, 0x00, /* IntervalTime */
	TEST_SECURITYLEVEL, /* SecurityLevel */
	TEST_KEYIDMODE, /* KeyIdMode */
	TEST_KEYSOURCE, /* KeySource */
	TEST_KEYINDEX /* KeyIndex */
};

uint8_t ref_hwme_set_req[] = {
	SPI_HWME_SET_REQUEST,
	3,
	TEST_HWATTRIBUTE,
	1,
	TEST_HWATTRIBUTEVALUE
};

uint8_t ref_hwme_get_req[] = {
	SPI_HWME_GET_REQUEST,
	1,
	TEST_HWATTRIBUTE
};

uint8_t ref_hwme_haes_req[] = {
	SPI_HWME_HAES_REQUEST,
	17,
	TEST_HAESMODE,
	TEST_HAESDATA
};

uint8_t	ref_tdme_setsfr_req[] = {
	SPI_TDME_SETSFR_REQUEST,
	3,
	TEST_SFRPAGE,
	TEST_SFRADDRESS,
	TEST_SFRVALUE
};

uint8_t	ref_tdme_getsfr_req[] = {
	SPI_TDME_GETSFR_REQUEST,
	2,
	TEST_SFRPAGE,
	TEST_SFRADDRESS
};

uint8_t	ref_tdme_testmode_req[] = {
	SPI_TDME_TESTMODE_REQUEST,
	1,
	TEST_TESTMODE
};

uint8_t	ref_tdme_set_req[] = {
	SPI_TDME_SET_REQUEST,
	1,
	TEST_TDMEATTRIBUTE,
	1,
	TEST_TDMEATTRIBUTEVALUE
};

uint8_t	ref_tdme_txpkt_req[] = {
	SPI_TDME_TXPKT_REQUEST,
	3+TEST_MSDULENGTH,
	TDME_TXD_APPENDED,
	TEST_SEQUENCENUM,
	TEST_MSDULENGTH,
	TEST_MSDU
};

uint8_t	ref_tdme_lotlk_req[] = {
	SPI_TDME_LOTLK_REQUEST,
	2,
	TEST_CHANNEL,
	0
};

int verify_command(
	const uint8_t *buf,
	size_t len,
	uint8_t *response,
	struct ca821x_dev *pDeviceRef
)
{
	uint8_t *reference_buffer = NULL;
	int reference_len = 0;
	printf("CmdId %#02x ", buf[0]);
	/* Set reference buffer */
	switch (buf[0]) {
	case SPI_MCPS_DATA_REQUEST:
		reference_buffer = ref_mcps_data_req;
		break;
	case SPI_MCPS_PURGE_REQUEST:
		reference_buffer = ref_mcps_purge_req;
		break;
	case SPI_MLME_ASSOCIATE_REQUEST:
		reference_buffer = ref_mlme_associate_req;
		break;
	case SPI_MLME_ASSOCIATE_RESPONSE:
		reference_buffer = ref_mlme_associate_resp;
		break;
	case SPI_MLME_DISASSOCIATE_REQUEST:
		reference_buffer = ref_mlme_disassociate_req;
		break;
	case SPI_MLME_GET_REQUEST:
		reference_buffer = ref_mlme_get_req;
		break;
	case SPI_MLME_ORPHAN_RESPONSE:
		reference_buffer = ref_mlme_orphan_resp;
		break;
	case SPI_MLME_RESET_REQUEST:
		reference_buffer = ref_mlme_reset_req;
		break;
	case SPI_MLME_RX_ENABLE_REQUEST:
		reference_buffer = ref_mlme_rx_enable_req;
		break;
	case SPI_MLME_SCAN_REQUEST:
		reference_buffer = ref_mlme_scan_req;
		break;
	case SPI_MLME_SET_REQUEST:
		reference_buffer = ref_mlme_set_req;
		break;
	case SPI_MLME_START_REQUEST:
		reference_buffer = ref_mlme_start_req;
		break;
	case SPI_MLME_POLL_REQUEST:
		reference_buffer = ref_mlme_poll_req;
		break;
	case SPI_HWME_SET_REQUEST:
		reference_buffer = ref_hwme_set_req;
		break;
	case SPI_HWME_GET_REQUEST:
		reference_buffer = ref_hwme_get_req;
		break;
	case SPI_HWME_HAES_REQUEST:
		reference_buffer = ref_hwme_haes_req;
		break;
	case SPI_TDME_SETSFR_REQUEST:
		reference_buffer = ref_tdme_setsfr_req;
		break;
	case SPI_TDME_GETSFR_REQUEST:
		reference_buffer = ref_tdme_getsfr_req;
		break;
	case SPI_TDME_TESTMODE_REQUEST:
		reference_buffer = ref_tdme_testmode_req;
		break;
	case SPI_TDME_SET_REQUEST:
		reference_buffer = ref_tdme_set_req;
		break;
	case SPI_TDME_TXPKT_REQUEST:
		reference_buffer = ref_tdme_txpkt_req;
		break;
	case SPI_TDME_LOTLK_REQUEST:
		reference_buffer = ref_tdme_lotlk_req;
		break;
	default:
		printf(ANSI_COLOR_RED "Invalid downstream CmdId: %#02x" ANSI_COLOR_RESET ", ", buf[0]);
		return -1;
	}
	/* Get buffer length from data length +cmdid +len bytes */
	if (reference_buffer)
		reference_len = reference_buffer[1] + 2;
	if (len != reference_len) {
		printf(ANSI_COLOR_RED "Command length does not match reference" ANSI_COLOR_RESET "\n");
		printf("result  reference\n");
		printf("%2u      %2d\n", len, reference_len);
		return -1;
	} else {
		/* Length matches */
		if (memcmp(buf, reference_buffer, reference_len)) {
			/* buffers differ */
			printf(ANSI_COLOR_RED "Output failed" ANSI_COLOR_RESET "\n");
			printf("result  reference\n");
			for(int i = 0; i < reference_len; i++) {
				if(buf[i] != reference_buffer[i]) {
					printf(ANSI_COLOR_RED);
				}
				printf("%02x      %02x\n" ANSI_COLOR_RESET, buf[i], reference_buffer[i]);
			}
			return -1;
		} else {
			/* buffers match */
			printf(ANSI_COLOR_GREEN "Output verified" ANSI_COLOR_RESET ", ");
		}
	}
	return 0;
}

void print_result(uint8_t result)
{
	printf("result: ");
	if (result) {
		printf(ANSI_COLOR_RED);
	} else {
		printf(ANSI_COLOR_GREEN);
	}
	printf("%#02x\n" ANSI_COLOR_RESET, result);
}

/******************************************************************************/
/***************************************************************************//**
 * \brief Main function for function test module
 *******************************************************************************
 * Tests the cascoda_api command functions to ensure that they produce valid
 * command frames for the transceiver.
 *******************************************************************************
 * \return TODO
 *******************************************************************************
 ******************************************************************************/
int api_functions_test(void)
{
	uint8_t ret, msdu_buffer[TEST_MSDULENGTH];
	uint8_t msduhandle=TEST_MSDUHANDLE;
	uint8_t pibattributelength=1, pibattributevalue=TEST_CHANNEL;
	uint8_t pollinterval[2]={0,0};
	uint8_t hwattributelength=1, hwattributevalue=TEST_HWATTRIBUTEVALUE;
	uint8_t haesdata[16];
	uint8_t sfrvalue, tdmeattributevalue=TEST_TDMEATTRIBUTEVALUE;
	uint8_t sequencenum=TEST_SEQUENCENUM, txpktlength=TEST_MSDULENGTH;
	uint8_t testchannel=TEST_CHANNEL, rxtxb=0, lo_vals[3];
	struct FullAddr full_address;
	struct ca821x_dev test_dev;
	struct SecSpec test_secspec = {
		.SecurityLevel = TEST_SECURITYLEVEL,
		.KeyIdMode = TEST_KEYIDMODE,
		.KeySource = {TEST_KEYSOURCE},
		.KeyIndex = TEST_KEYINDEX
	};
	/* TODO: check return */
	ca821x_api_init(&test_dev);
	test_dev.ca821x_api_downstream = verify_command;
	/* Call each API downstream, check constructed packet */
	full_address.AddressMode = MAC_MODE_LONG_ADDR;
	full_address.PANId[0] = 0x5C;
	full_address.PANId[1] = 0xCA;
	memcpy(full_address.PANId, (uint8_t[]) {TEST_PANID}, sizeof(full_address.PANId));
	memcpy(full_address.Address, (uint8_t[]) {TEST_DSTADDR}, sizeof(full_address.Address));
	memcpy(msdu_buffer, (uint8_t[]) {TEST_MSDU}, TEST_MSDULENGTH);
	memcpy(haesdata, (uint8_t[]) {TEST_HAESDATA}, sizeof(haesdata));
	printf("Testing MCPS_DATA_request()... ");
	ret = MCPS_DATA_request(
		MAC_MODE_SHORT_ADDR,
		full_address.AddressMode,
		GETLE16(full_address.PANId),
		(union MacAddr*)full_address.Address,
		TEST_MSDULENGTH,
		msdu_buffer,
		TEST_MSDUHANDLE,
		0x00,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing MCPS_PURGE_request_sync()... ");
	ret = MCPS_PURGE_request_sync(
		&msduhandle,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_ASSOCIATE_request()... ");
	ret = MLME_ASSOCIATE_request(
		TEST_CHANNEL,
		full_address.AddressMode,
		GETLE16(full_address.PANId),
		(union MacAddr*)full_address.Address,
		0,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_ASSOCIATE_response()... ");
	ret = MLME_ASSOCIATE_response(
		full_address.Address,
		0xCA01,
		0,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_DISASSOCIATE_request()... ");
	ret = MLME_DISASSOCIATE_request(
		full_address,
		0,
		0,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_SET_request_sync()... ");
	ret = MLME_SET_request_sync(
		TEST_PIBATTRIBUTE,
		0,
		pibattributelength,
		&pibattributevalue,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_GET_request_sync()... ");
	ret = MLME_GET_request_sync(
		TEST_PIBATTRIBUTE,
		0,
		&pibattributelength,
		&pibattributevalue,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_ORPHAN_response()... ");
	ret = MLME_ORPHAN_response(
		full_address.Address,
		0xCA01,
		0,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_RESET_request_sync()... ");
	ret = MLME_RESET_request_sync(
		0,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_RX_ENABLE_request_sync()... ");
	ret = MLME_RX_ENABLE_request_sync(
		0,
		0xA000000A,
		0xB000000B,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_SCAN_request()... ");
	ret = MLME_SCAN_request(
		ACTIVE_SCAN,
		0x07FFF800,
		0x01,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_START_request_sync()... ");
	ret = MLME_START_request_sync(
		GETLE16(full_address.PANId),
		TEST_CHANNEL,
		0x0F,
		0x0F,
		0,
		0,
		0,
		&test_secspec,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing MLME_POLL_request_sync()... ");
	ret = MLME_POLL_request_sync(
		full_address,
		pollinterval,
		&test_secspec,
		&test_dev
	);
	print_result(ret);
	printf("Testing HWME_SET_request_sync()... ");
	ret = HWME_SET_request_sync(
		TEST_HWATTRIBUTE,
		1,
		&hwattributevalue,
		&test_dev
	);
	print_result(ret);
	printf("Testing HWME_GET_request_sync()... ");
	ret = HWME_GET_request_sync(
		TEST_HWATTRIBUTE,
		&hwattributelength,
		&hwattributevalue,
		&test_dev
	);
	print_result(ret);
	printf("Testing HWME_HAES_request_sync()... ");
	ret = HWME_HAES_request_sync(
		TEST_HAESMODE,
		haesdata,
		&test_dev
	);
	print_result(ret);
	printf("Testing TDME_SETSFR_request_sync()... ");
	ret = TDME_SETSFR_request_sync(
		TEST_SFRPAGE,
		TEST_SFRADDRESS,
		TEST_SFRVALUE,
		&test_dev
	);
	print_result(ret);
	printf("Testing TDME_GETSFR_request_sync()... ");
	ret = TDME_GETSFR_request_sync(
		TEST_SFRPAGE,
		TEST_SFRADDRESS,
		&sfrvalue,
		&test_dev
	);
	print_result(ret);
	printf("Testing TDME_TESTMODE_request_sync()... ");
	ret = TDME_TESTMODE_request_sync(
		TEST_TESTMODE,
		&test_dev
	);
	print_result(ret);
	printf("Testing TDME_SET_request_sync()... ");
	ret = TDME_SET_request_sync(
		TEST_TDMEATTRIBUTE,
		1,
		&tdmeattributevalue,
		&test_dev
	);
	print_result(ret);
	printf("Testing TDME_TXPKT_request_sync()... ");
	ret = TDME_TXPKT_request_sync(
		TDME_TXD_APPENDED,
		&sequencenum,
		&txpktlength,
		msdu_buffer,
		&test_dev
	);
	print_result(ret);
	printf("Testing TDME_LOTLK_request_sync()... ");
	ret = TDME_LOTLK_request_sync(
		&testchannel,
		&rxtxb,
		lo_vals,
		lo_vals+1,
		lo_vals+2,
		&test_dev
	);
	print_result(ret);
	printf("Function test complete\n");
	return 0;
}

int main(void)
{
	return api_functions_test();
}