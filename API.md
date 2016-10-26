# API Reference
Include and instantiate the TheThingsNetwork class. The constructor initialize the library with the Streams it should communicate with. It also sets the value of the spreading factor, the front-side Bus and the frequency plan.

## Class: TheThingsNetwork

```c
#include <TheThingsNetwork.h>

TheThingsNetwork ttn(Stream& modemStream, Stream& debugStream, fp_ttn_t fp, uint8_t sf = 7, uint8_t fsb = 2);
```

- `Stream& modemStream`: Stream for the LoRa modem (for The Things Node/Uno use `Serial1` and data rate `57600`).
- `Stream& debugStream`: Stream to write debug logs to (for The Things Node/Uno use `Serial` and data rate `9600`).
- `fp_ttn_fp fp`: The frequency plan: `TTN_FP_EU868` or `TTN_FP_US915` depending on the region you deploy in.
- `uint8_t sf = 7`: Optional custom spreading factor. Can be `7` to `12` for `TTN_FP_EU868` and `7` to `12` for `TTN_FP_US915`. Defaults to `7`.
- `uint8_t fsb = 2`: Optional custom front-side bus. Can be `1` to `8`. Defaults to `2`.

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
Total airtime: 0.00 s
```

See the [DeviceInfo](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/DeviceInfo/DeviceInfo.ino) example.

## Method: onMessage
Sets a function which will be called to process incoming messages.

```c
void onMessage(void (*cb)(const byte* payload, size_t length, port_t port));
```

- `const byte* payload`: Bytes received.
- `size_t length`: Number of bytes.
- `port_t port`: The port addressed.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: join
Activate the device via OTAA (default).

```c
bool join(const char *appEui, const char *appKey, int8_t retries = -1, uint32_t retryDelay = 10000);
bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
```

- `const char *appEui`: Application EUI the device is registered to.
- `const char *appKey`: Application Key assigned to the device.
- `int8_t retries = -1`: Number of times to retry after failed or unconfirmed join. Defaults to `-1` which means infinite.
- `uint32_t retryDelay = 10000`: Delay in ms between attempts. Defaults to 10 seconds.

Returns `true` or `false` depending on whether it received confirmation that the activation was successful before the maximum number of attempts.

Call the method without the first two arguments if the device's LoRa module comes with pre-flashed values.

## Method: personalize
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

See the [ABP](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/ABP/ABP.ino) example.

## Method: sendBytes
Send a message to the application using raw bytes.

```c
int sendBytes(const byte* payload, size_t length, port_t port = 1, bool confirm = false);
```

- `const byte* payload `: Bytes to send.
- `size_t length`: The number of bytes. Use `sizeof(payload)` to get it.
- `port_t port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation. Defaults to `false`.

Returns a success or error code and logs the related error message:

* `-1`: Send command failed.
* `-2`: Time-out.
* `1`: Successful transmission.
* `2`: Successful transmission. Received \<N> bytes
* `-10`: Unexpected response: \<Response>

See the [Send](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Send/Send.ino) example.

Also in sendBytes, due to TTN's 30 second fair access policy, we update the airtime each time we uplink a message. This airtime is based on a lot of variables but the most important ones are the spreading factor and the size of the message, the higher it is the less messages you can send in 30 seconds (SF7 around 500 messages of 8 bytes, SF12 around 20 messages of 8 bytes).

## Method: poll
Calls `sendBytes()` with `{ 0x00 }` as payload to poll for incoming messages.

- `port_t port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation.

Returns the result of `sendBytes()`.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: provision
Sets the information needed to activate the device via OTAA, without actually activating. Call join() without the first 2 arguments to activate.

```c
bool provision(const char *appEui, const char *appKey);
```

- `const char *appEui`: Application Identifier for the device.
- `const char *appKey`: Application Key assigned to the device.
