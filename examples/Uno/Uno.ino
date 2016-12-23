#include <TheThingsNetwork.h>

// These keys are for https://ttn.fyi/activate
// Replace them if you want to use your own app
const char *appEui = "70B3D57EF0001CEE";
const char *appKey = "F2E5C891560FF9CE24AD56E1A69B85DF";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan REPLACE_ME

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

#define PORT_SETUP 1
#define PORT_INTERVAL 2

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Test LoRa module and turn on LED if it works
  ttn.showStatus();
  digitalWrite(LED_BUILTIN, HIGH);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- TTN: STATUS");
  ttn.showStatus();

  debugSerial.println("-- TTN: JOIN");
  ttn.join(appEui, appKey);

  debugSerial.println("-- SEND: SETUP");
  sendData(PORT_SETUP);
}

void loop() {
  delay(60000);

  debugSerial.println("-- SEND: INTERVAL");
  sendData(PORT_INTERVAL);
}

void sendData(uint8_t port) {ttn.showStatus();
  ttn.showStatus();

  byte payload[] = { 0x00 };
  ttn.sendBytes(payload, sizeof(payload), 10 + port);
}
