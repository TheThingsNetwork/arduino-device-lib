# API Reference

## Class: TheThingsNetwork
Include and instantiate the TheThingsNetwork class to communicate over The Things Network.

The constructor initialize the library with the Streams it should communicate with. It also sets the value of the spreading factor, the front-side Bus and the frequency plan.

```c
#include <TheThingsNetwork.h>

TheThingsNetwork ttn(Stream& modemStream, Stream& debugStream, fp_ttn_t fp, int sf = 7, int fsb = 2);
```

- `Stream& modemStream`: Stream for the LoRa modem (for The Things Node/Uno use `Serial1` and data rate `57600`).
- `Stream& debugStream`: Stream to write debug logs to (for The Things Node/Uno use `Serial` and data rate `9600`).
- `fp_ttn_fp fp`: The frequency plan: `TTN_FP_EU868` or `TTN_FP_US915` depending on the region you deploy in.
- `int sf = 7`: Optional custom spreading factor. Can be `7` to `12` for `TTN_FP_EU868` and `7` to `12` for `TTN_FP_US915`. Defaults to `7`.
- `int fsb = 2`: Optional custom front-side bus. Can be `1` to `8`. Defaults to `2`.

## Method: showStatus
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
Band: 868
Data Rate: 5
RX Delay 1: 1000
RX Delay 2: 2000
```

See the [DeviceInfo](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/DeviceInfo/DeviceInfo.ino) example.

## Method: onMessage
Sets a function which will be called to process incoming messages.

```c
void onMessage(void (*cb)(const byte* payload, int length, int port));
```

- `const byte* payload`: Bytes received.
- `int length`: Number of bytes.
- `int port`: The port addressed.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: join
Activate the device via OTAA (default).

```c
bool join(const byte appEui[8], const byte appKey[16], int retries = -1, long int retryDelay = 10000);
bool join(int retries = -1, long int retryDelay = 10000);
```

- `const byte appEui[8]`: Application EUI the device is registered to.
- `const byte appKey[16]`: Application Key assigned to the device.
- `int retries = -1`: Number of times to retry after failed or unconfirmed join. Defaults to `-1` which means infinite.
- `long int retryDelay = 10000`: Delay in ms between attempts. Defaults to 10 seconds.

Returns `true` or `false` depending on whether it received confirmation that the activation was successful before the maximum number of attempts.

Call the method without the first two arguments if the device's LoRa module comes with pre-flashed values.

## Method: personalize
Activate the device via ABP.

```c
bool personalize(const byte devAddr[4], const byte nwkSKey[16], const byte appSKey[16]);
bool personalize();
```

- `const byte devAddr[4]`: Device Address assigned to the device.
- `const byte nwkSKey[16]`: Network Session Key assigned to the device for identification.
- `const byte appSKey[16]`: Application Session Key assigned to the device for encryption.

Returns `true` or `false` depending on whether the activation was successful.

Call the method with no arguments if the device's LoRa module comes with pre-flashed values.

See the [ABP](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/ABP/ABP.ino) example.

## Method: sendBytes
Send a message to the application using raw bytes.

```c
int sendBytes(const byte* payload, int length, int port = 1, bool confirm = false);
```

- `const byte* payload `: Bytes to send.
- `int length`: The number of bytes. Use `sizeof(payload)` to get it.
- `int port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation. Defaults to `false`.

Returns a success or error code and logs the related error message: 

* `-1`: Send command failed.
* `-2`: Time-out.
* `1`: Successful transmission.
* `2`: Successful transmission. Received \<N> bytes
* `-10`: Unexpected response: \<Response> 

See the [Send](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Send/Send.ino) example.

## Method: poll
Calls `sendBytes()` with `{ 0x00 }` as payload to poll for incoming messages.

- `int port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation.

Returns the result of `sendBytes()`.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: provision
Sets the information needed to activate the device via OTAA, without actually activating. Call join() without the first 2 arguments to activate.

```c
bool provision(const byte appEui[8], const appKey[16]);
```

- `const byte appEui[8]`: Application Identifier for the device.
- `const byte appKey[16]`: Application Key assigned to the device.
