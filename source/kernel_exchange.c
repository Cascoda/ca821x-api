#include <stdlib.h>
#include <fcntl.h>

#include "../include/cascoda_api.h"

/******************************************************************************/

#define DriverFilePath "/dev/ca8210_test"

/******************************************************************************/

static int ca8210_test_int_exchange(
	const uint8_t *buf,
	size_t len,
	uint8_t *response,
	void *pDeviceRef
);

/******************************************************************************/

static int DriverFileDescriptor;

/******************************************************************************/

int kernel_exchange_init(void)
{
	DriverFileDescriptor = open(DriverFilePath, O_RDWR);
	cascoda_api_downstream = ca8210_test_int_exchange;
}

static void ca8210_test_int_write(uint8_t *buf, size_t len)
{
	int returnvalue, remaining = len;

	do {
		returnvalue = write(DriverFileDescriptor, buf+len-remaining, remaining);
		if(returnvalue > 0)
			remaining -= returnvalue;
	} while (remaining > 0);
}

static int ca8210_test_int_exchange(
	const uint8_t *buf,
	size_t len,
	uint8_t *response,
	void *pDeviceRef
)
{
	int status, Rx_Length;
	ca8210_test_int_write(buf, len);

	if ((buf[0] & SPI_SYN) && response) {
		do {
			Rx_Length = read(DriverFileDescriptor, response, NULL);
		} while (Rx_Length == 0);
	}

	return 0;
}
