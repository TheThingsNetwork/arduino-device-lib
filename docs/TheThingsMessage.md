# API Reference

## Class: `TheThingsMessage`
The `TheThingsMessage` class provides structs for sensor and application data you can encode and decode as bytes.

```c
#include <TheThingsMessage.h>
```

## Type: `devicedata_t`

Create a struct of this type using `api_DeviceData_init_default` as defaults.

```c
devicedata_t data = api_DeviceData_init_default;
```

Then in your `setup()` function select what fields of the struct should be included when encoding it as bytes:

```c
data.has_motion = true;
data.has_water = false;
data.has_temperature_celcius = true;
data.has_temperature_fahrenheit = true;
data.has_humidity = true;
```

In your `loop()` function read your sensors and set the fields of the struct:

```c
data.motion = true;
data.water = 682;
data.temperature_celcius = 30
data.temperature_fahrenheit = 86;
data.humidity = 97;
```

### Additional analog readings

You can also add other analog readings.

> **TODO:** Document how this works and include in example.

## Method: `encodeDeviceData`

Encode the message you want to send.

```c
static void encodeDeviceData(devicedata_t *data, byte **buffer, size_t *size);
```

- `devicedata_t *data`: Structure containing typical fields that devices send.
- `byte **buffer`: Bytes to send.
- `size_t *size`: The number of bytes to send.

Usage:

```c
byte *buffer;
size_t size;
TheThingsMessage::encodeDeviceData(&data, &buffer, &size);
```

## Method: `decodeAppData`

Decode the message you received.

```c
static bool decodeAppData(appdata_t *receiveData, const byte *payload, size_t length);
```

- `appdata_t *receiveData`: Structure containing all the message we can interpret.
- `const byte *payload`: Bytes received.
- `size_t *length`: The number of bytes.

Usage:

```c
const byte *payload;
size_t length;
TheThingsMessage::decodeAppData(&receiveData, payload, length);
```
