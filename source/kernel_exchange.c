#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

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
static pthread_t rx_thread;
static pthread_mutex_t rx_mutex = PTHREAD_MUTEX_INITIALIZER;

/******************************************************************************/

static void *ca8210_test_int_read_worker(void *arg)
{
	uint8_t rx_buf[512];
	size_t rx_len;
	/* TODO: while not told to exit? */
	while (1) {
		pthread_mutex_lock(&rx_mutex);
		rx_len = read(DriverFileDescriptor, rx_buf, 0);
		pthread_mutex_unlock(&rx_mutex);
		if (rx_len > 0) {
			cascoda_downstream_dispatch(rx_buf, rx_len);
		}
	}

	return NULL;
}

int kernel_exchange_init(void)
{
	int ret;
	pthread_attr_t attrs;

	DriverFileDescriptor = open(DriverFilePath, O_RDWR);

	cascoda_api_downstream = ca8210_test_int_exchange;

	ret = pthread_create(&rx_thread, &attrs, ca8210_test_int_read_worker, NULL);
	return ret;
}

static void ca8210_test_int_write(const uint8_t *buf, size_t len)
{
	int returnvalue, remaining = len;

	do {
		returnvalue = write(DriverFileDescriptor, buf+len-remaining, remaining);
		if (returnvalue > 0)
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
	uint8_t isSynchronous = ((buf[0] & SPI_SYN) && response);

	if(isSynchronous) pthread_mutex_lock(&rx_mutex);	//Enforce synchronous write then read

	ca8210_test_int_write(buf, len);

	if (isSynchronous) {
		do {
			Rx_Length = read(DriverFileDescriptor, response, NULL);
		} while (Rx_Length == 0);
		pthread_mutex_unlock(&rx_mutex);
	}
	

	return 0;
}
