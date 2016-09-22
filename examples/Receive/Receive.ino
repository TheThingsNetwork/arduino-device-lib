#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const byte appEui[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const byte appKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn;

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  ttn.init(loraSerial, debugSerial);

  // Set callback for incoming messages
  ttn.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop() {
  debugSerial.println("-- LOOP");

  // Send single byte to poll for incoming messages
  ttn.poll();

  delay(10000);
}

void message(const byte* payload, int length, int port) {
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(length) + " bytes on port " + String(port) + ":");

  for (int i = 0; i < length; i++) {
    debugSerial.print(" " + String(payload[i]));
  }

  debugSerial.println();
}