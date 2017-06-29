# API Reference

The `CayenneLPP` class enables Arduino devices to encode data with the Cayenne Low Power Payload (LPP). [Read more about Cayenne LPP](https://mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload)

## Class: `CayenneLPP`

Include and instantiate the CayenneLPP class. The constructor takes the size of the allocated buffer. Depending on the LoRa frequency plan and data rate used, the maximum payload varies. It's safe to send up to 51 bytes of payload.

```c
#include <CayenneLPP.h>

CayenneLPP lpp(uint8_t size);
```

- `uint8_t size`: The maximum payload size to send, e.g. `51`.

## Example

```c
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
CayenneLPP lpp(51);

lpp.reset();
lpp.addTemperature(1, 22.5);
lpp.addBarometricPressure(2, 1073.21);
lpp.addGPS(3, 52.37365, 4.88650, 2);

ttn.sendBytes(lpp.getBuffer(), lpp.getSize());
```

See the [CayenneLPP](https://github.com/TheThingsNetwork/arduino-device-lib/blob/master/examples/CayenneLPP/CayenneLPP.ino) example. 

## Method: `reset`

Resets the buffer.

```c
void reset(void);
```

## Method: `getSize`

Returns the size of the buffer.

```c
uint8_t getSize(void);
```

## Method: `getBuffer`

Returns a pointer to the buffer.

```c
uint8_t *getBuffer(void);
```

## Method: `copy`

Copies the internal buffer to a specified buffer and returns the copied size.

```c
uint8_t copy(uint8_t *buffer);
```

## Methods: `add...`

Add data to the buffer. The `channel` parameter acts as a key for the data field. The data fields you send are dynamic; you can selectively send data as long as the channel matches.

```c
uint8_t addDigitalInput(uint8_t channel, uint8_t value);
uint8_t addDigitalOutput(uint8_t channel, uint8_t value);

uint8_t addAnalogInput(uint8_t channel, float value);
uint8_t addAnalogOutput(uint8_t channel, float value);

uint8_t addLuminosity(uint8_t channel, uint16_t lux);
uint8_t addPresence(uint8_t channel, uint8_t value);
uint8_t addTemperature(uint8_t channel, float celsius);
uint8_t addRelativeHumidity(uint8_t channel, float rh);
uint8_t addAccelerometer(uint8_t channel, float x, float y, float z);
uint8_t addBarometricPressure(uint8_t channel, float hpa);
uint8_t addGyrometer(uint8_t channel, float x, float y, float z);
uint8_t addGPS(uint8_t channel, float latitude, float longitude, float meters);
```
