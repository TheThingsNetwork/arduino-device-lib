#include <TheThingsNetwork.h>
#include <TheThingsMessage.h>

// Set your AppEUI, AppKey and the frequency plan
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";
const ttn_fp_t freqPlan = // Use either TTN_FP_EU868; or TTN_FP_US915; here

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

sensordata_t sensorData = api_SensorData_init_default;

void setup()
{
  pinMode(TTN_PIN_LED, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  ttn.onMessage(message);

  sensorData.has_motion = true;
  sensorData.has_water = true;
  sensorData.has_temperature_celcius = false;
  sensorData.has_temperature_fahrenheit = false;
  sensorData.has_humidity = false;
}

void loop() {
  // Read sensors
  sensorData.motion = digitalRead(TTN_PIN_LED) == HIGH;
  sensorData.water = 682;

  // Encode data
  byte *buffer;
  size_t size;

  // Send standard message on port 100
  TheThingsMessage::encodeSensorData(&sensorData, &buffer, &size);
  ttn.sendBytes(buffer, size, 100);

  delay(10000);
}

void message(const byte* payload, size_t length, port_t port) {
  //standard message always received on port 100 or more
  if (port >= 100) {
    appdata_t appData = api_AppData_init_default;
    TheThingsMessage::decodeAppData(&appData, payload, length);
  }
}