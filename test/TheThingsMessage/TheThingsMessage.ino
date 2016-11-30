#include <TheThingsMessage.h>

sensordata_t data = api_SensorData_init_default;

void setup() {
  data.has_motion = true;
  data.has_water = false;
  data.has_temperature_celcius = true;
  data.has_temperature_fahrenheit = true;
  data.has_humidity = true;
}

void loop() {
  data.motion = true;
  data.water = 682;
  data.temperature_celcius = 30;
  data.temperature_fahrenheit = 86;
  data.humidity = 97;

  byte *buffer;
  size_t size;

  TheThingsMessage::encodeSensorData(&data, &buffer, &size);
}
