# cascoda-api
This is Cascoda's IEEE 802.15.4 API library used for communication with the following devices:

ca8210

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