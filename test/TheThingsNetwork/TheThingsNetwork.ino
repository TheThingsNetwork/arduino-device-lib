#include <TheThingsNetwork.h>

const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, TTN_FP_EU868);

TheThingsNetwork ttn2(loraSerial, debugSerial, TTN_FP_US915, 10);

TheThingsNetwork ttn3(loraSerial, debugSerial, TTN_FP_AS923_925, 10, 4);


void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  ttn.provision(appEui, appKey);

  ttn.join(appEui, appKey);

  ttn.join(appEui, appKey, 3);

  ttn.join(appEui, appKey, 3, 60000);

  ttn.join();

  ttn.join(3);

  ttn.join(3, 60000);

  ttn.personalize(devAddr, nwkSKey, appSKey);

  ttn.personalize();

  ttn.showStatus();

  ttn.onMessage(message);
}

void loop()
{
  byte payload[1] = {0x00};
  ttn.sendBytes(payload, sizeof(payload));

  ttn.sendBytes(payload, sizeof(payload), 2);

  ttn.sendBytes(payload, sizeof(payload), 2, true);

  ttn.poll();

  ttn.poll(2);

  ttn.poll(2, true);
}

void message(const uint8_t *payload, size_t length, port_t port) {}
