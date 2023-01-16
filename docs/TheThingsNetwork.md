# API Reference

The `TheThingsNetwork` class enables Arduino devices with supported LoRaWAN modules to communicate via The Things Network. Currently supported LoRaWAN modules are the Microchip RN2483 and the RN2903.

## Class: `TheThingsNetwork`

Include and instantiate the TheThingsNetwork class. The constructor initializes the library with the Streams it should communicate with. It also sets the value of the spreading factor, the frequency plan, and the frequency sub-band.

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

Performs a hardware reset of the RN module. Input parameter is the pin to which the reset pin from the module is connected. This does clear saved state, e.g., provisioned keys.

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

## Method: `getVersion`

Gets the response from a `sys get ver` command (i.e. the hardware model, the the software version, etc.).

```c
size_t getVersion(char *buffer, size_t size);
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

Sets a function that will be called to process incoming messages. You'll want to do this in your `setup()` function and then define a `void (*cb)(const byte* payload, size_t length, port_t port)` function somewhere else in your sketch.

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
- `int8_t retries = -1`: Number of times to retry after failed or unconfirmed join. Defaults to `-1`, which means infinite.
- `uint32_t retryDelay = 10000`: Delay in ms between attempts. Defaults to 10 seconds.
- `lorawan_class = CLASS_A`: The LoRaWAN class to use for downlink message reception.

Returns `true` or `false` depending on whether it received confirmation that the activation was successful before the maximum number of attempts.

Call the method without the first two arguments if the device's LoRa module comes with pre-flashed values.

## Method: `personalize`

Activate the device via ABP.

```c
bool personalize(const char *devAddr, const char *nwkSKey, const char *appSKey, bool reset_first);
bool personalize();
```

- `const char *devAddr`: Device Address assigned to the device.
- `const char *nwkSKey`: Network Session Key assigned to the device for identification.
- `const char *appSKey`: Application Session Key assigned to the device for encryption.
- `bool reset_first`: Soft reset the module before performing any other action. Default is `true`.

Returns `true` or `false` depending on whether the activation was successful.

Call the method with no arguments if the device's LoRa module comes with pre-flashed values.

See the [SendABP](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/SendABP/SendABP.ino) example.

## Method: `setClass`

Change the downlink receive LoRaWAN Class. Class C is only supported in firmware version 1.0.5 and up. For other firmware versions, this method will have no effect.

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
- `bool confirm = false`: Whether to ask for confirmation—defaults to `false`. If confirmation fails, the method will return the error code `TTN_ERROR_UNEXPECTED_RESPONSE`.
- `uint8_t sf = 0`: Override the spreading factor (SF). If the default value `0` is passed, the SF is not changed from the constructor or previous value.

Returns a success or error code and logs the related error message:

* `TTN_ERROR_SEND_COMMAND_FAILED`: Send command failed.
* `TTN_SUCCESSFUL_TRANSMISSION`: Successful transmission.
* `TTN_SUCCESSFUL_RECEIVE`: Successful transmission. Received \<N> bytes
* `TTN_UNSUCCESSFUL_RECEIVE`: if we sent a confirmed message but did not get an acknowledgment
* `TTN_ERROR_UNEXPECTED_RESPONSE`: Unexpected response: \<Response>

See the [SendOTAA](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/SendOTAA/SendOTAA.ino) example.

## Method: `poll`

Calls `sendBytes()` with `{ 0x00 }` as payload to poll for incoming messages.

```c
int8_t poll(port_t port = 1, bool confirm = false);
```

- `port_t port = 1`: The port to address. Defaults to `1`.
- `bool confirm = false`: Whether to ask for confirmation.
- `bool modem_only = false`: (only class A) Whether to avoid sending an empty frame and only listen to the modem's incoming message.

Returns the result of `sendBytes()`, with one difference:
* `TTN_UNSUCCESSFUL_RECEIVE`: In class C or class A `modem_only` poll, we only listened, and there was no incoming message.

The `modem_only` option for poll may be useful if the sending procedure did not terminate yet, e.g., we sent a confirmed message and did not get a response. In such a case, the modem will independently retry. With `modem_only` we thus just listen to the modem output to verify the status of the further attempts.

See the [Receive](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/Receive/Receive.ino) example.

## Method: `provision`

Sets the information needed to activate the device via OTAA, without actually activating. Call join() without the first 2 arguments to activate.

```c
bool provision(const char *appEui, const char *appKey, bool reset_first);
```

- `const char *appEui`: Application Identifier for the device.
- `const char *appKey`: Application Key assigned to the device.
- `bool reset_first`: Soft reset the module before performing any other action. Default is `true`.

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

Sets the time interval for the link check process to be triggered. The next uplink will include a Link Check Request MAC command when the interval expires. This method should be called after joining has been completed.

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

# Additional for operation

## Method: `getVDD`

Returns the voltage in millivolt (mV) measured by the RN2xxx LoRa module. It's for information only since we don't know how it's calculated but looks accurate.

```c
uint16_t getVDD();
```

## Method: `getStatus`

Returns the status of the RN2xxx modem's state machine in one of the nine possible states of `ttn_modem_status_t`. Unfortunately, due to a firmware bug, this does not work for RN2493 modems with firmware < 1.05. If unsuccessful, the method returns `TTN_MODEM_READ_ERROR`.

```c
enum ttn_modem_status_t getStatus()
```

Possible return codes are:

- `TTN_MODEM_READ_ERR` could not read out the status
- `TTN_MODEM_IDLE` modem idle, ready for next tx
- `TTN_MODEM_TX` modem transmitting
- `TTN_MODEM_BEFORE_RX` waiting time between tx and rx1 windows
- `TTN_MODEM_RX1` RX1 window open
- `TTN_MODEM_BEFORE_RX2` waiting time between the two rx windows
- `TTN_MODEM_RETRY_DELAY` waiting before retry again
- `TTN_MODEM_APB_DELAY` APB join delay
- `TTN_MODEM_C_RX1` RX1 window open in class C
- `TTN_MODEM_C_RX2` RX2 window open in class C


## Method: `getLastError`

Returns the last error code encountered by the RN2xxx modem. The error code may be one of the twelve possible errors of `ttn_response_code_t`. Call this method after an unsuccessful execution.

```c
ttn_response_code_t getLastError();
```

For example, suppose you perform a send via `sendbytes` and get a `TTN_ERROR_SEND_COMMAND_FAILED`. In that case, you can call this method and get a `TTN_ERR_BUSY`, which means the modem already has a message to send in the buffer.

Possible error codes are:

 - `TTN_OK` nothing actually went wrong.
 - `TTN_ERROR_BUSY` busy sending a message
 - `TTN_ERROR_FRAME_COUNTER_ERROR` if the frame counter rolled over
 - `TTN_ERROR_INVALID_CLASS` selected LoRaWan class is invalid
 - `TTN_ERROR_INVALID_LENGTH` data length is not available for the current configuration
 - `TTN_ERROR_INVALID_PARAMETER` invalid parameter specified
 - `TTN_ERROR_NO_KEY_INTITIALIZED` network keys not initialized
 - `TTN_ERROR_MAC_PAUSE` mac level has been paused
 - `TTN_ERROR_NO_KEY_MULTICAST` multicast keys not set
 - `TTN_ERROR_NO_FREE_CHANNEL` all channels exhausted their duty cycle, so we can not send
 - `TTN_ERROR_NOT_JOINED` modem did not join a network
 - `TTN_ERROR_SILENT` if the module is in a Silent Immediately state
 - `TTN_ERROR_ERR` other unspecified error 

## Method: `getFCU`

Returns the next used uplink frame counter. In LoRaWan, this counter must match on both sides, server, and end-node, for a transmission to be successful. Please note that although the counter is 32bit, the frame itself contains only the lowest 16bits of the message. Nonetheless, the message is encrypted using all 32bits of the FCU in the key. Therefore, it _must_ be up to date and in sync. If the readout is unsuccessful, the method returns 0. However, the first frame after a reset is also set to 0.

```c
uint32_t getFCU();
```

## Method: `setFCU`

Sets the FCU as of above. The method returns true if successful.

```c
bool setFCU(uint32_t fcu);
```

## Method: `getFCD`

Returns the next expected downlink frame counter. In LoRaWan, this counter must match on both sides, server, and end-node, for a transmission to be successful. Please note that although the counter is 32bit, the frame itself contains only the lowest 16bits of the message. Nonetheless, the message is encrypted using all 32bits of the FCD in the key. Therefore, it _must_ be up to date and in sync. If the readout is unsuccessful, the method returns 0. However, the first frame after a reset is also sent with frame number 0.

```c
uint32_t getFCD();
```

## Method: `setFCD`

Sets the FCD as of above. The method returns true if successful.

```c
bool setFCD(uint32_t fcd);
```

## Method: `getDR`

Returns the data rate that the modem will use for the next uplink frame. If the readout is unsuccessful, the method returns -1.

```c
int8_t getDR();
```

## Method: `setDR`

Sets the data rate to use for the next uplink frame; see `getDR`. This is mainly useful for operating with adaptive data rate (ADR) in `off`; see `setADR`. If ADR is on, it will set the DR for the next uplink, but the value for the successive transmissions may change according to the ADR protocol. The range of valid DR values depends on the frequency plan. The method returns true if successful.

```c
bool setDR(uint8_t dr);
```

## Method: `getPowerIndex`

Returns the power index that the LoRa modem will use for the next uplink frame. If the readout is unsuccessful, the method returns -1.

```c
int8_t getPowerIndex();
```

## Method: `setPowerIndex`

Sets the power index to use for the next uplink frame; see `getPowerIndex`. This is mainly useful for operating with adaptive data rate (ADR) in `off`; see `setADR`. If ADR is on, it will set the power index for the next uplink. Still, the value for the following transmissions may change according to the ADR protocol. The range of valid values depends on modem hardware and the frequency plan. The method returns true if successful.

```c
bool setPowerIndex(uint8_t index);
```

## Method: `setChannel`

The TTN module can store multiple channel configurations. With this method, you can configure a logical channel of the modem by defining frequency in `Hz`, minimum and maximum usable data rates for the channel. Data rates depend on the used frequency plan and must be between 0 and 15. The method returns true if a configuration is successful.

```c
bool setChannel(uint8_t channel, uint32_t frequency = 0l, uint8_t dr_min = 255, uint8_t dr_max = 255);
```

Alternatively, you can only re-set the data rates for the channel by leaving (omitting) frequency to 0.
 
```c
setChannel(2, dr_min = 0, dr_max = 4);
```
  
Or, you can only re-set the frequency for the channel by leaving (omitting) dr_* to 255.

```c
setChannel(2, 863500000);
```

However, the whole configuration must have been set at least once to be successful in both cases.

## Method: `setRX2Channel`

In some plans, the TTN module stores a common RX2 downlink channel, which is used for RX2 window communication. With this method, you can configure this channel by defining frequency in `Hz` and its data rates. The valid data rates depend on the used frequency plan. The method returns true if a configuration is successful.

```c
bool setRx2Channel(uint32_t frequency, uint8_t dr);
```

## Method: `getChannelStatus`

The TTN module can store multiple channel configurations. This method returns true if the channel is enabled for transmission and/or receiving.

```c
bool getChannelStatus (uint8_t channel);
```

## Method: `setChannelStatus`

Sets the channel status of the selected channel number; see `getChannelStatus`. If set to true and the channel is configured, the modem may use the channel for transmission. The method returns true if successful.

```c
bool setChannelStatus (uint8_t channel, bool status);
```

## Method: `setChannelDCycle`

Sets the channel maximum allowed duty cycle of a selected channel number in `%`. Depending on regional regulations, the duty cycle may limit the maximum frequency use to a percentage in time. With this method, we set the ratio of transmission time for the selected channel, e.g., `1%` with 1 second transmission time means the radio has to pause for 99 seconds on this channel before retransmitting. The method returns true if successful.

```c
bool setChannelDCycle (uint8_t channel, float duty_cycle);
```

## Method: `setADR`

We can manually disable or enable the adaptive data rate (ADR) algorithm on the device with this method. It independently adapts the transmission data rate (and power) in runtime if active. If disabled, we must select power index and data rate manually, see `setDR` and `setPowerIndex`. The method returns true if successful.

```c
bool setADR(bool adr);
```

## Method: `setRX1Delay`

When transmitting in LoRaWan, we usually operate on a TX window and two RX windows. This setting defines how long the modem has to wait before opening the channel for listening for RX window 1. RX window two is hardcoded to be one second after RX1. This parameter is configurable between 1 and 15 seconds in `ms`. By default, we have a delay of 1000 ms, and you should not change this if you do not change the network server accordingly. Longer window delays may be helpful with slow, unresponsive Internet connectivity. The method returns true if successful.

```c
bool setRX1Delay(uint16_t delay);
```

## Method: `checkValidModuleConnected`

Checks if a valid module is connected to the configured serial port. Useful to check for connectivity with a supported module before performing any other actions.

```c
bool checkValidModuleConnected(bool autobaud_first);
```

- `bool autobaud_first`: Perform a call to `autoBaud()` before checking connection. Default is `false`.

Returns:

* `false` if no response was received (i.e. `needsHardReset` is `true`)
* `false` if the module is invalid (unsupported), i.e. **not** one of the following:
    * `RN2483`
    * `RN2483A`
    * `RN2903`
    * `RN2903AS`
* `true` if the module responded (i.e. `needsHardReset` is `false`) and is valid (supported).

See the [CheckModule](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/CheckModule/CheckModule.ino) example.

# Additional for statistics 

## Method: `getRSSI`

Returns the relative signal strength of the last received frame in `dBm`. Typical values are between -70dBm (very strong) and -136dBm (weak/sensitivity threshold). If the readout is unsuccessful, the method returns -255.

```c
int16_t getRSSI();
```

## Method: `getSNR`

Returns the signal to noise ratio of the last received frame in `dB`. Note that LoRa also allows negative values for SNR. Typical values are between +20dB (very clear) and -20dB (weak/sensitivity threshold). If the readout is unsuccessful, the method returns -128.

```c
int8_t getSNR();
```

## Method: `getFrequency`

Returns the channel frequency of the last received frame in `Hz`. If the readout is unsuccessful, the method returns 0.

```c
uint32_t getFrequency();
```

## Method: `getWatchDogTimer`

Returns the LoRa modem's internal watch-dog timer in `ms`. If this time is exceeded, the modem resets. If instead, the readout is unsuccessful, the method returns 0.

```c
uint32_t getWatchDogTimer();
```

## Method: `getBW`

Returns the used channel bandwidth of the last received frame in `kHz`. If the readout is unsuccessful, the method returns 0.

```c
uint8_t getBW();
```

## Method: `getCR`

Returns the used code rate of the last received frame in `4/x`. If the readout is unsuccessful, the method returns 0.

```c
uint8_t getCR();
```

## Method: `getPower`

Returns the power of the last incoming transmission in `dBm`. If the readout is unsuccessful, the method returns -128.

```c
int8_t getPower();
```
