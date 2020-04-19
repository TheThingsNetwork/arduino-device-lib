#include <TheThingsNetwork.h>
#include <TheThingsMessage.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan REPLACE_ME

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

devicedata_t data = api_DeviceData_init_default;

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  // Select what fields to include in the encoded message
  data.has_motion = true;
  data.has_water = false;
  data.has_temperature_celcius = true;
  data.has_temperature_fahrenheit = true;
  data.has_humidity = true;
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Read the sensors
  data.motion = true;
  data.water = 682;
  data.temperature_celcius = 30;
  data.temperature_fahrenheit = 86;
  data.humidity = 97;

  // Encode the selected fields of the struct as bytes
  byte *buffer;
  size_t size;
  TheThingsMessage::encodeDeviceData(&data, &buffer, &size);

  ttn.sendBytes(buffer, size);

  delay(10000);
}
