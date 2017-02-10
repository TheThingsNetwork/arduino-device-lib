#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

//define AnalogPin for sensor
#define LightPin A0

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan REPLACE_ME

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  pinMode(LightPin, INPUT);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  ttn.showStatus();
  debugSerial.println("Setup for The Things Network complete");
}

void loop()
{

  uint16_t light = analogRead(LightPin);

  // Split word (16 bits) into 2 bytes of 8
  byte payload[2];
  payload[0] = highByte(light);
  payload[1] = lowByte(light);

  debugSerial.print("Transmitting Light level: ");
  debugSerial.println(light);

  ttn.sendBytes(payload, sizeof(payload));

  delay(20000);
}
