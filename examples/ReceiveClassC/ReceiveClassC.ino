#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

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

  // Set callback for incoming messages
  ttn.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey, -1, 10000, CLASS_C);

  // Class C RX only takes affect after a TX
  uint8_t payload[] = {0x00};
  ttn.sendBytes(payload, 1);
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Check for received data.
  ttn.poll();

  // When using Class C we can poll as quickly as we can, as we only check the serial buffer.
  //delay(1000);
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (int i = 0; i < size; i++)
  {
    debugSerial.print(" " + String(payload[i]));
  }

  debugSerial.println();
}
