# API Reference

The `TheThingsNetwork` class enables Arduino devices with supported LoRaWAN modules to communicate via The Things Network. Currently supported LoRaWAN modules are the Microchip RN2483 and the RN2903.

## Class: `TheThingsNetwork`

Include and instantiate the TheThingsNetwork class. The constructor initialize the library with the Streams it should communicate with. It also sets the value of the spreading factor, the frequency plan and the frequency sub-band.

```c
#include <TheThingsNetwork.h>

TheThingsNetwork ttn(Stream& modemStream, Stream& debugStream, fp_ttn_t fp, uint8_t sf = 7, uint8_t fsb = 2);
```

- `Stream& modemStream`: Stream for the LoRa modem ([see notes](https://www.thethingsnetwork.org/docs/devices/arduino/usage.html)).
- `Stream& debugStream`: Stream to write debug logs to ([see notes](https://www.thethingsnetwork.org/docs/devices/arduino/usage.html)).
- `fp_ttn_fp fp`: The frequency plan: `TTN_FP_EU868`, `TTN_FP_US915`, `TTN_FP_AS920_923`, `TTN_FP_AS923_925` or `TTN_FP_KR920_923` depending on the region you deploy in. See [the wiki](https://www.thethingsnetwork.org/wiki/LoRaWAN/Frequencies/Frequency-Plans).
- `uint8_t sf = 7`: Optional custom spreading factor. Can be `7` to `10` for `TTN_FP_US915` and `7` to `12` for other frequency plans. Defaults to `7`.
- `uint8_t fsb = 2`: Optional custom frequency subband. Can be `1` to `8`. Defaults to `2` (for US915).

## Method: `reset`

Performs a software reset of the RN module. This does not clear saved state, e.g. provisioned keys.

```c
void reset(bool adr);
```

- `bool adr`: Enable/disable Adaptive Data Rate.

## Method: `hardReset`

Performs a hardware reset of the RN module. Input parameter is the pin which the reset pin from the module is connected to. This does clear saved state, e.g. provisioned keys.

```c
void hardReset(uint8_t resetPin);
```

- `uint8_t resetPin`: The output pin that is connected to the module's reset pin. The output pin should be configured as output and set to high by the user.

## Method: `getHardwareEui`

Gets the unique hardware EUI, often used as the DevEUI.

```c
size_t getHardwareEui(char *buffer, size_t size);
```

## Method: `getAppEui`

Gets the provisioned AppEUI. The AppEUI is set using `provision()` or `join()`.

```c
size_t getAppEui(char *buffer, size_t size);
```

## Method: `showStatus`

Writes information about the device and LoRa module to `debugStream`.

```c
void showStatus();
```

Will write something like:

```bash
EUI: 0004A30B001B7AD2
Battery: 3223
AppEUI: 70B3D57EF000001C
DevEUI: 0004A30B001B7AD2
Data Rate: 5
RX Delay 1: 1000
RX Delay 2: 2000
```

See the [DeviceInfo](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/DeviceInfo/DeviceInfo.ino) example.

## Method: `onMessage`

Sets a function which will be called to process incoming messages. You'll want to do this in your `setup()` function and then define a `void (*cb)(const byte* payload, size_t length, port_t port)` function somewhere else in your sketch.

```c
void onMessage(void (*cb)(const byte* payload, size_t length, port_t port));
```

- `const byte* payload`: Bytes received.
- `size_t length`: Number of bytes.
- `port_t port`: The port addressed.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: `join`

Activate the device via OTAA (default).

```c
bool join(const char *appEui, const char *appKey, int8_t retries = -1, uint32_t retryDelay = 10000, lorawan_class = CLASS_A);
bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
```

- `const char *appEui`: Application EUI the device is registered to.
- `const char *appKey`: Application Key assigned to the device.
- `int8_t retries = -1`: Number of times to retry after failed or unconfirmed join. Defaults to `-1` which means infinite.
- `uint32_t retryDelay = 10000`: Delay in ms between attempts. Defaults to 10 seconds.
- `lorawan_class = CLASS_A`: The LoRaWAN class to use for downlink message reception.

Returns `true` or `false` depending on whether it received confirmation that the activation was successful before the maximum number of attempts.

Call the method without the first two arguments if the device's LoRa module comes with pre-flashed values.

## Method: `personalize`

Activate the device via ABP.

```c
bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey);
bool personalize();
```

- `const char *devAddr`: Device Address assigned to the device.
- `const char *nwkSKey`: Network Session Key assigned to the device for identification.
- `const char *appSKey`: Application Session Key assigned to the device for encryption.

Returns `true` or `false` depending on whether the activation was successful.

Call the method with no arguments if the device's LoRa module comes with pre-flashed values.

See the [SendABP](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/SendABP/SendABP.ino) example.

## Method: `setClass`

Change the downlink receive LoRaWAN Class. Class C is only supported in firmware version 1.0.5 and up. For other firmware versions this method will have no effect.

```c
bool setClass(lorawan_class p_lw_class);
```

- `lorawan_class p_lw_class`: The LoRaWAN class. Either `CLASS_A` or `CLASS_C`.

Returns `true` if the change was successful, or `false` if not successful.

The receive window only opens after a transmit. Therefore Class C receive will only start after calling `sendBytes()`.

See the [ReceiveClassC](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/ReceiveClassC/ReceiveClassC.ino) example.

## Method: `sendBytes`

Send a message to the application using raw bytes.

```c
ttn_response_t sendBytes(const byte* payload, size_t length, port_t port = 1, bool confirm = false, uint8_t sf = 0);
```

- `const byte* payload `: Bytes to send.
- `size_t length`: The number of bytes. Use `sizeof(payload)` to get it.
- `port_t port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation. Defaults to `false`. If confirmation fails, the method will return error code `TTN_ERROR_UNEXPECTED_RESPONSE`.
- `uint8_t sf = 0`: Override the spreading factor (SF). If the default value `0` is passed, the SF is not changed from the constructor or previous value.

Returns a success or error code and logs the related error message:

* `TTN_ERROR_SEND_COMMAND_FAILED`: Send command failed.
* `TTN_SUCCESSFUL_TRANSMISSION`: Successful transmission.
* `TTN_SUCCESSFUL_RECEIVE`: Successful transmission. Received \<N> bytes
* `TTN_ERROR_UNEXPECTED_RESPONSE`: Unexpected response: \<Response>

See the [SendOTAA](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/SendOTAA/SendOTAA.ino) example.

## Method: `poll`

Calls `sendBytes()` with `{ 0x00 }` as payload to poll for incoming messages.

```c
int8_t poll(port_t port = 1, bool confirm = false);
```

- `port_t port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation.

Returns the result of `sendBytes()`.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: `provision`

Sets the information needed to activate the device via OTAA, without actually activating. Call join() without the first 2 arguments to activate.

```c
bool provision(const char *appEui, const char *appKey);
```

- `const char *appEui`: Application Identifier for the device.
- `const char *appKey`: Application Key assigned to the device.

## Method: `sleep`

Sleep the LoRa module for a specified number of milliseconds.

```c
void sleep(unsigned long mseconds);
```

- `unsigned long mseconds`: number of milliseconds to sleep.

## Method: `wake`

Wake up the LoRa module from sleep before the expiration of the defined time.

```c
void wake();
```

## Method: `linkCheck`

Sets the time interval for the link check process to be triggered. When the interval expires, the next uplink will include a Link Check Request MAC command. When using this method, it should be called after joining has completed.

```c
void linkCheck(uint16_t seconds);
```

- `uint16_t seconds`: the time interval in seconds. A value of 0 will disable the link check process.

## Method: `getLinkCheckGateways`

Gets the number of gateways that successfully received the last Link Check Request frame.

```c
uint8_t getLinkCheckGateways();
```

## Method: `getLinkCheckMargin`

Gets the demodulation margin as received in the last Link Check Answer frame.

```c
uint8_t getLinkCheckMargin();
```

## Method: `getVDD`

Returns the voltage in millivolt (mV) measured by the RN2xxx LoRa module. It's for information only since we don't know how it's measured but looks like accurate.

```c
uint16_t getVDD();
```
