# API Reference

The `TheThingsNetwork` class enables Arduino devices with supported LoRaWAN modules to communicate via The Things Network. Currently supported LoRaWAN modules are the Microchip RN2483 and the RN2903.

## Class: `TheThingsNetwork`

Include and instantiate the TheThingsNetwork class. The constructor initialize the library with the Streams it should communicate with. It also sets the value of the spreading factor, the frequency plan and the frequency sub-band.

```c
#include <TheThingsNetwork.h>

TheThingsNetwork ttn(Stream& modemStream, Stream& debugStream, fp_ttn_t fp, uint8_t sf = 7, uint8_t fsb = 2);
```

- `Stream& modemStream`: Stream for the LoRa modem (see comments at the end of this document).
- `Stream& debugStream`: Stream to write debug logs to (see comments at the end of this document).
- `fp_ttn_fp fp`: The frequency plan: `TTN_FP_EU868` or `TTN_FP_US915` depending on the region you deploy in.
- `uint8_t sf = 7`: Optional custom spreading factor. Can be `7` to `12` for `TTN_FP_EU868` and `7` to `10` for `TTN_FP_US915`. Defaults to `7`.
- `uint8_t fsb = 2`: Optional custom frequency sub-band. Can be `1` to `8`. Defaults to `2` (for US915).

## Method: `reset`

Performs a software reset of the RN module. This does not clear saved state, e.g. provisioned keys.

```c
void showStatus();
```

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
Band: 868
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
bool join(const char *appEui, const char *appKey, int8_t retries = -1, uint32_t retryDelay = 10000);
bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
```

- `const char *appEui`: Application EUI the device is registered to.
- `const char *appKey`: Application Key assigned to the device.
- `int8_t retries = -1`: Number of times to retry after failed or unconfirmed join. Defaults to `-1` which means infinite.
- `uint32_t retryDelay = 10000`: Delay in ms between attempts. Defaults to 10 seconds.

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

See the [ABP](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/ABP/ABP.ino) example.

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

See the [Send](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Send/Send.ino) example.

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

# Comments
## Serial ports
The RN2483 and RN2903 make use of a serial interface to communicate with your device's main processor. Serial interfaces are similar to the RS232 serial port on older computers. The most microcontrollers have hardware Universal Asynchronous Receiver/Transmitters (UARTs) or also called HardwareSerial ports. Serial communication is offloaded to these UARTs so that the main processor does not have to waste time on this slow task. A UART can only support one serial interface at a time, and most processors only have one or two UARTs. Therefore if you already used the available UARTs to communicate with, for example, your computer and a GPS, you do not have any left for the RN2483/RN2903. 

When you run out of UARTs, you can still use serial communication handled in software. In this case it is not offloaded to dedicated hardware, and the main processor needs to handle the communication. This is called Software Serial. Depending on how the software serial is implemented, you can have as many as you want, as long as you have free GPIO pins on your device. Note that not all GPIO pins support all types of software serial. Examples of software serial is the default Arduino [SoftwareSerial](https://www.arduino.cc/en/Reference/SoftwareSerial) library, and the [AltSoftSerial](https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html) library.

### Stream objects
In the Arduino world serial ports are abstracted as stream objects. All stream objects have the same functions, even if lower down they use different physical connections (UART, SoftwareSerial, AltSoftSerial). This library does not care what you use, as long as it extends the Stream object.

The Stream objects need to be initialized at the correct baud rates at the start of your `setup()` function. See [our examples](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples) for more details. For example:
```
  loraSerial.begin(57600);
  debugSerial.begin(9600);
```

### TheThingsUno
TheThingsUno is basically the same as an Arduino Leonardo with Serial1 connected to the RN2483/RN2903. Therefore if you use an Arduino Leonardo, and connect your RN2483/RN2903 to the Serial1 pins, you can use the same configuration than for TheThingsUno.

At the top of your sketch use
```
#define loraSerial Serial1
#define debugSerial Serial
```
And in your `setup()` function use
```
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  ...
}
```

### SODAQ One
At the top of your sketch use
```
#define loraSerial Serial1
#define debugSerial SerialUSB
```
And in your `setup()` function use
```
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  ...
}
```

### Arduino Uno, Arduino Nano or other devices using SoftwareSerial
The Arduino Uno only has one hardware serial port which is used to communicate over USB to the computer. When connecting an RN2483/RN2903 to the Arduino Uno, one has to make use of SoftwareSerial. If you connected the RN2483/RN2903 to the Arduino using the same pinout as [described on the forum](https://www.thethingsnetwork.org/forum/t/how-to-build-your-first-ttn-node-arduino-rn2483/1574), you can make use of the following code.

At the top of your sketch use
```
#include <SoftwareSerial.h>

#define debugSerial Serial

SoftwareSerial loraSerial(10, 11); // RX, TX
```
And in your `setup()` function use
```
void setup()
{
  loraSerial.begin(9600);
  debugSerial.begin(9600);
  ...
}
```

SoftwareSerial does not operate correctly at high baud rates. We normally use it at 9600 baud. Because the RN2483 and RN2903 normally operates at 57600 baud, we need to switch it to 9600 baud so that we can communicate with it using 9600 baud. This is done automatically inside TheThingsNetwork Arduino library. Changing of the baud rate of the RN2483/RN2903 is not always very reliable. Power cycling the device, or a reset, might be necessary.

If you connected the RN2483/RN2903 to different pins on the Arduino, you can change the line `SoftwareSerial loraSerial(10, 11); // RX, TX` to specify the correct RX and TX pins (from the Arduino's perspective).

When using the [AltSoftSerial](https://github.com/PaulStoffregen/AltSoftSerial) library the pins you can use for software serial is fixed according to which device you use. You therefore do not have a choice which pins to use, and you may lose some other functionality, but this library seems to be more stable than the default SoftwareSerial library.