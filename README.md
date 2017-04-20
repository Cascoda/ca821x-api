# ca821x-api
This is Cascoda's IEEE 802.15.4 API library used for communication with the CA821X family of devices.

The API models the SAP interface of the 802.15.4 specification (MCPS+MLME) as well as two proprietary entities, the HWME (Hardware Management Entity) and the TDME (Test & Debug Management Entity). The API defines two main types of command; synchronous and asynchronous. Synchronous command functions will not return until the corresponding response has been received from the device whereas asynchronous functions will return immediately. A set of callbacks are provided for processing asynchronous responses:
```
struct cascoda_api_callbacks {
	...
}
```
An instance of this struct must be created in the user's application and registered using the function
```
int cascoda_register_callbacks(struct cascoda_api_callbacks in_callbacks);
```

All downstream commands will be sent by calling the function pointer:
```
extern int (*ca821x_api_downstream)(
	const uint8_t *buf,
	size_t len,
	uint8_t *response,
	void *pDeviceRef
);
```
This pointer must be populated with an implementation conforming to this prototype. The function should transmit the contents of `buf` and populate `response` with whatever synchronous response is received, if `buf` contains a synchronous command. If `buf` contains an asynchronous command, `response` can be ignored.  
`pDeviceRef` is passed through to this function from the API call at the top level. It can be used to identify the CA-821X instance being controlled (e.g. passing a private data reference, device ID etc).
